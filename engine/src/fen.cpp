#include "stockfih/fen.hpp"

#include <array>
#include <cctype>
#include <sstream>
#include <vector>

namespace stockfih {
namespace {

std::optional<Piece> charToPiece(char symbol) {
  const Color color = std::isupper(static_cast<unsigned char>(symbol)) ? Color::White
                                                                       : Color::Black;
  switch (std::tolower(static_cast<unsigned char>(symbol))) {
    case 'p': return Piece{PieceType::Pawn, color};
    case 'n': return Piece{PieceType::Knight, color};
    case 'b': return Piece{PieceType::Bishop, color};
    case 'r': return Piece{PieceType::Rook, color};
    case 'q': return Piece{PieceType::Queen, color};
    case 'k': return Piece{PieceType::King, color};
    default: return std::nullopt;
  }
}

std::optional<Square> parseAlgebraic(std::string_view text) {
  if (text.size() != 2) return std::nullopt;
  const int file = text[0] - 'a';
  const int rank = text[1] - '1';
  if (!isOnBoard(file, rank)) return std::nullopt;
  return makeSquare(file, rank);
}

std::vector<std::string> split(const std::string& text) {
  std::istringstream stream(text);
  std::vector<std::string> fields;
  std::string field;
  while (stream >> field) fields.push_back(field);
  return fields;
}

bool placePieces(Board& board, const std::string& placement) {
  int rank = 7;
  int file = 0;
  for (const char symbol : placement) {
    if (symbol == '/') {
      if (file != kBoardSize) return false;
      file = 0;
      --rank;
      if (rank < 0) return false;
    } else if (std::isdigit(static_cast<unsigned char>(symbol))) {
      file += symbol - '0';
      if (file > kBoardSize) return false;
    } else {
      const std::optional<Piece> piece = charToPiece(symbol);
      if (!piece || file >= kBoardSize) return false;
      board.set(makeSquare(file, rank), *piece);
      ++file;
    }
  }
  return rank == 0 && file == kBoardSize;
}

bool parseCastling(Board& board, const std::string& field) {
  if (field == "-") {
    board.setCastlingRights(kNoCastling);
    return true;
  }
  std::uint8_t rights = kNoCastling;
  for (const char symbol : field) {
    switch (symbol) {
      case 'K': rights |= kWhiteKingSide; break;
      case 'Q': rights |= kWhiteQueenSide; break;
      case 'k': rights |= kBlackKingSide; break;
      case 'q': rights |= kBlackQueenSide; break;
      default: return false;
    }
  }
  board.setCastlingRights(rights);
  return true;
}

std::optional<int> parseNonNegativeInt(const std::string& field) {
  if (field.empty()) return std::nullopt;
  for (const char symbol : field) {
    if (!std::isdigit(static_cast<unsigned char>(symbol))) return std::nullopt;
  }
  return std::stoi(field);
}

}  // namespace

std::optional<Board> parseFen(std::string_view fen) {
  const std::vector<std::string> fields = split(std::string(fen));
  if (fields.size() != 6) return std::nullopt;

  Board board;
  board.clear();

  if (!placePieces(board, fields[0])) return std::nullopt;

  if (fields[1] == "w") {
    board.setSideToMove(Color::White);
  } else if (fields[1] == "b") {
    board.setSideToMove(Color::Black);
  } else {
    return std::nullopt;
  }

  if (!parseCastling(board, fields[2])) return std::nullopt;

  if (fields[3] == "-") {
    board.setEnPassantSquare(kNoSquare);
  } else {
    const std::optional<Square> square = parseAlgebraic(fields[3]);
    if (!square) return std::nullopt;
    board.setEnPassantSquare(*square);
  }

  const std::optional<int> halfmove = parseNonNegativeInt(fields[4]);
  const std::optional<int> fullmove = parseNonNegativeInt(fields[5]);
  if (!halfmove || !fullmove) return std::nullopt;
  board.setHalfmoveClock(*halfmove);
  board.setFullmoveNumber(*fullmove);

  return board;
}

}  // namespace stockfih
