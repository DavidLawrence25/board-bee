#ifndef BOARD_BEE_LIBS_ANSI_H_
#define BOARD_BEE_LIBS_ANSI_H_

#include <libs/aliases.h>

#include <array>
#include <ostream>
#include <sstream>

namespace rose::ansi {

struct Color {
  u8 red, green, blue;
};

enum class BasicPalette { kBlack, kRed, kGreen, kYellow,
                          kBlue, kMagenta, kCyan, kWhite };

inline u8 ColorCube6(const u8 r, const u8 g, const u8 b) {
  if (r > 5 || g > 5 || b > 5) {
    std::stringstream error_msg;
    // We have to cast to u16's so it doesn't print them as chars.
    error_msg << '(' << static_cast<u16>(r) << ", " << static_cast<u16>(g)
              << ", " << static_cast<u16>(b)
              << ") is not a valid coordinate on the 6x6x6 color cube";
    throw std::runtime_error(error_msg.str());
  }
  return 16 + 36 * r + 6 * g + b;
}

class CtrlSequence {
 public:
  static constexpr size_t kMaxArgs = 5;
  static_assert(kMaxArgs != 0);

  enum class FunctionCode {
    kCursorUp = 'A',
    kCursorDown = 'B',
    kCursorForward = 'C',
    kCursorBack = 'D',
    kCursorNextLine = 'E',
    kCursorPreviousLine = 'F',
    kCursorHorizontalAbsolute = 'G',
    kCursorPosition = 'H',
    kCursorShow = 'h',
    kCursorHide = 'l',
    kEraseInDisplay = 'J',
    kEraseInLine = 'K',
    kScrollUp = 'S',
    kScrollDown = 'T',
    kSaveCursorPosition = 's',
    kRestoreCursorPosition = 'u',
    kSetGraphicsRendition = 'm'
  };

  static CtrlSequence CursorUp(u16 n = 1);
  static CtrlSequence CursorDown(u16 n = 1);
  static CtrlSequence CursorForward(u16 n = 1);
  static CtrlSequence CursorBack(u16 n = 1);
  static CtrlSequence CursorNextLine(u16 n = 1);
  static CtrlSequence CursorPreviousLine(u16 n = 1);
  static CtrlSequence CursorHorizontalAbsolute(u16 column = 1);
  static CtrlSequence CursorPosition(u16 column = 1, u16 row = 1);
  static CtrlSequence CursorShow();
  static CtrlSequence CursorHide();
  static CtrlSequence ClearCursorToScreenEnd();
  static CtrlSequence ClearScreenStartToCursor();
  static CtrlSequence ClearScreen();
  static CtrlSequence ClearScreenAndBuffer();
  static CtrlSequence ClearCursorToLineEnd();
  static CtrlSequence ClearLineStartToCursor();
  static CtrlSequence ClearLine();
  static CtrlSequence ScrollUp(u16 n = 1);
  static CtrlSequence ScrollDown(u16 n = 1);
  static CtrlSequence SaveCursorPosition();
  static CtrlSequence RestoreCursorPosition();
  static CtrlSequence SgrReset();
  static CtrlSequence SgrBold();
  static CtrlSequence SgrItalic();
  static CtrlSequence SgrUnderline();
  static CtrlSequence SgrTextColor(BasicPalette color, bool bright = false);
  static CtrlSequence SgrTextColor(u8 color_code);
  static CtrlSequence SgrTextColor(Color color);
  static CtrlSequence SgrBgColor(BasicPalette color, bool bright = false);
  static CtrlSequence SgrBgColor(u8 color_code);
  static CtrlSequence SgrBgColor(Color color);

  CtrlSequence(const FunctionCode code,
               const std::array<opt<u16>, kMaxArgs> &args)
      : function_code_(code), args_(args) {}
  CtrlSequence(const CtrlSequence &other) = default;
  CtrlSequence &operator=(const CtrlSequence &other) = default;
  CtrlSequence(CtrlSequence &&other) = default;
  CtrlSequence &operator=(CtrlSequence &&other) = default;

  explicit operator str() const;
  friend std::ostream &operator<<(std::ostream &out, const CtrlSequence &seq);

 private:
  FunctionCode function_code_;
  std::array<opt<u16>, kMaxArgs> args_;
};

}  // namespace rose::ansi

#endif  // BOARD_BEE_LIBS_ANSI_H_
