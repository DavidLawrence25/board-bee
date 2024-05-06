#include <libs/ansi.h>

namespace rose::ansi {

CtrlSequence CtrlSequence::CursorUp(const u16 n) {
  return {FunctionCode::kCursorUp, {n}};
}

CtrlSequence CtrlSequence::CursorDown(const u16 n) {
  return {FunctionCode::kCursorDown, {n}};
}

CtrlSequence CtrlSequence::CursorForward(const u16 n) {
  return {FunctionCode::kCursorForward, {n}};
}

CtrlSequence CtrlSequence::CursorBack(const u16 n) {
  return {FunctionCode::kCursorBack, {n}};
}

CtrlSequence CtrlSequence::CursorNextLine(const u16 n) {
  return {FunctionCode::kCursorNextLine, {n}};
}

CtrlSequence CtrlSequence::CursorPreviousLine(const u16 n) {
  return {FunctionCode::kCursorPreviousLine, {n}};
}

CtrlSequence CtrlSequence::CursorHorizontalAbsolute(const u16 column) {
  return {FunctionCode::kCursorHorizontalAbsolute, {column}};
}

CtrlSequence CtrlSequence::CursorPosition(const u16 column, const u16 row) {
  return {FunctionCode::kCursorPosition, {column, row}};
}

CtrlSequence CtrlSequence::CursorShow() {
  return {FunctionCode::kCursorShow, {}};
}

CtrlSequence CtrlSequence::CursorHide() {
  return {FunctionCode::kCursorHide, {}};
}

CtrlSequence CtrlSequence::ClearCursorToScreenEnd() {
  return {FunctionCode::kEraseInDisplay, {0}};
}

CtrlSequence CtrlSequence::ClearScreenStartToCursor() {
  return {FunctionCode::kEraseInDisplay, {1}};
}

CtrlSequence CtrlSequence::ClearScreen() {
  return {FunctionCode::kEraseInDisplay, {2}};
}

CtrlSequence CtrlSequence::ClearScreenAndBuffer() {
  return {FunctionCode::kEraseInDisplay, {3}};
}

CtrlSequence CtrlSequence::ClearCursorToLineEnd() {
  return {FunctionCode::kEraseInLine, {0}};
}

CtrlSequence CtrlSequence::ClearLineStartToCursor() {
  return {FunctionCode::kEraseInLine, {1}};
}

CtrlSequence CtrlSequence::ClearLine() {
  return {FunctionCode::kEraseInLine, {2}};
}

CtrlSequence CtrlSequence::ScrollUp(const u16 n) {
  return {FunctionCode::kScrollUp, {n}};
}

CtrlSequence CtrlSequence::ScrollDown(const u16 n) {
  return {FunctionCode::kScrollDown, {n}};
}

CtrlSequence CtrlSequence::SaveCursorPosition() {
  return {FunctionCode::kSaveCursorPosition, {}};
}

CtrlSequence CtrlSequence::RestoreCursorPosition() {
  return {FunctionCode::kRestoreCursorPosition, {}};
}

CtrlSequence CtrlSequence::SgrReset() {
  return {FunctionCode::kSetGraphicsRendition, {0}};
}

CtrlSequence CtrlSequence::SgrBold() {
  return {FunctionCode::kSetGraphicsRendition, {1}};
}

CtrlSequence CtrlSequence::SgrItalic() {
  return {FunctionCode::kSetGraphicsRendition, {3}};
}

CtrlSequence CtrlSequence::SgrUnderline() {
  return {FunctionCode::kSetGraphicsRendition, {4}};
}

CtrlSequence CtrlSequence::SgrTextColor(const BasicPalette color,
                                        const bool bright) {
  const u16 color_code = static_cast<u16>(color) + (bright ? 90 : 30);
  return {FunctionCode::kSetGraphicsRendition, {color_code}};
}

CtrlSequence CtrlSequence::SgrTextColor(const u8 color_code) {
  return {FunctionCode::kSetGraphicsRendition, {38, 5, color_code}};
}

CtrlSequence CtrlSequence::SgrTextColor(Color color) {
  return {FunctionCode::kSetGraphicsRendition,
          {38, 2, color.red, color.green, color.blue}};
}

CtrlSequence CtrlSequence::SgrBgColor(const BasicPalette color,
                                      const bool bright) {
  const u16 color_code = static_cast<u16>(color) + (bright ? 100 : 40);
  return {FunctionCode::kSetGraphicsRendition, {color_code}};
}

CtrlSequence CtrlSequence::SgrBgColor(const u8 color_code) {
  return {FunctionCode::kSetGraphicsRendition, {48, 5, color_code}};
}

CtrlSequence CtrlSequence::SgrBgColor(Color color) {
  return {FunctionCode::kSetGraphicsRendition,
          {48, 2, color.red, color.green, color.blue}};
}

CtrlSequence::operator str() const {
  auto code = static_cast<char>(function_code_);
  if (code == 'h') return "\033[?25h";
  if (code == 'l') return "\033[?25l";
  std::stringstream sout;
  sout << "\033[";
  if (args_[0]) {
    sout << args_[0].value();
    for (u64 i = 1; i < kMaxArgs; ++i) {
      if (!args_[i]) break;
      sout << ';' << args_[i].value();
    }
  }
  sout << static_cast<char>(function_code_);
  return sout.str();
}

std::ostream &operator<<(std::ostream &out, const CtrlSequence &seq) {
  out << static_cast<str>(seq);
  return out;
}

}  // namespace rose::ansi
