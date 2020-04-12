// Copyright (c) 2020 CS126SP20. All rights reserved.

#include <snake/leaderboard.h>
#include <snake/player.h>
#include <sqlite_modern_cpp.h>

#include <string>
#include <vector>

namespace snake {

using std::string;
using std::vector;

// See examples: https://github.com/SqliteModernCpp/sqlite_modern_cpp/tree/dev

LeaderBoard::LeaderBoard(const string& db_path) : db_{db_path} {
  try {
  db_ << "CREATE TABLE if not exists leaderboard (\n"
         "  name  TEXT NOT NULL,\n"
         "  score INTEGER NOT NULL\n"
         ");";
  } catch (const std::exception& ex) {
    exit(1);
  }
}

void LeaderBoard::AddScoreToLeaderBoard(const Player& player) {
  try {
    db_ << "INSERT INTO leaderboard (name,score) values (?,?);" << player.name
        << player.score;
  } catch (const std::exception& ex) {
    exit(1);
  }
}

vector<Player> GetPlayers(sqlite::database_binder* rows) {
  vector<Player> players;

  for (auto&& row : *rows) {
    string name;
    size_t score;
    row >> name >> score;
    Player player = {name, score};
    players.push_back(player);
  }

  return players;
}

vector<Player> LeaderBoard::RetrieveHighScores(const size_t limit) {
  try {
    auto rows = db_ << "SELECT name,score FROM leaderboard ORDER BY score DESC LIMIT ?;"
                    << limit;

    return GetPlayers(&rows);
  } catch (const std::exception& ex) {
    exit(1);
  }
}

vector<Player> LeaderBoard::RetrieveHighScores(const Player& player,
                                               const size_t limit) {
  try {
    auto rows = db_ << "SELECT name,score FROM leaderboard WHERE name = ? ORDER BY score DESC LIMIT ?;"
        << player.name
        << limit;
    return GetPlayers(&rows);
  } catch (const std::exception& ex) {
    exit(1);
  }
}

}  // namespace snake
