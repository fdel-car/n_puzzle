#include "Node.hpp"
#include <cstring>
#include "Heuristics.hpp"
#include "Puzzle.hpp"

Node::Node(const std::vector<u_char> &firstGrid) {
  tiles.resize(Puzzle::totalSize);
  std::memcpy(tiles.data(), firstGrid.data(),
              sizeof(u_char) * Puzzle::totalSize);
  ID = std::string(tiles.begin(), tiles.end());
  gScore = 0;
  computeHeuristic();
  emptyTileCoords = getValueCoords(tiles, 0);
}

Node::Node(Node *parent, const std::array<int, 2> &emptyTileSwapDir)
    : parent(parent) {
  if (!parent) throw std::runtime_error("No parent given to a new child node.");
  tiles.resize(Puzzle::totalSize);
  std::memcpy(tiles.data(), (parent->tiles).data(),
              sizeof(u_char) * Puzzle::totalSize);
  gScore = parent->gScore + 1;

  // Move tile to the next position
  int oldX = parent->emptyTileCoords[0];
  int oldY = parent->emptyTileCoords[1];
  int newX = oldX + emptyTileSwapDir[0];
  int newY = oldY + emptyTileSwapDir[1];
  std::swap(tiles[oldX + oldY * Puzzle::N], tiles[newX + newY * Puzzle::N]);
  ID = std::string(tiles.begin(), tiles.end());
  emptyTileCoords = {newX, newY};
}

Node::~Node(void) {}

void Node::computeHeuristic(void) {
  hScore = Heuristics::manhattanDistance(tiles);
  fScore = gScore + hScore;
}

const std::array<int, 2> Node::getValueCoords(const std::vector<u_char> &tiles,
                                              u_char value) {
  for (int y = 0; y < Puzzle::N; y++) {
    for (int x = 0; x < Puzzle::N; x++) {
      if (tiles[x + y * Puzzle::N] == value) return {x, y};
    }
  }
  throw std::runtime_error("Looking for non existing value (" +
                           std::to_string(value) + ") inside the tiles.");
}

bool Node::operator==(const Node &rhs) const { return ID == rhs.ID; }

std::ostream &operator<<(std::ostream &os, const Node &node) {
  for (int y = 0; y < Puzzle::N; y++) {
    for (int x = 0; x < Puzzle::N; x++) {
      if (x != 0 && x != Puzzle::N) os << ' ';
      os << std::setw(Puzzle::nbrLength)
         << static_cast<int>(node.tiles[x + y * Puzzle::N]);
    }
    os << std::endl;
  }
  // // ID debug
  // for (u_char c : node.ID) {
  //   c += 48;
  //   os << c;
  // }
  return os;
}