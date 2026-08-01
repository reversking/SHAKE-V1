




#ifndef JSON_READER_H_INCLUDED
#define JSON_READER_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "json_features.h"
#include "value.h"
#endif
#include <deque>
#include <iosfwd>
#include <istream>
#include <stack>
#include <string>



#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#pragma pack(push)
#pragma pack()

namespace Json {







class JSON_API Reader {
public:
  using Char = char;
  using Location = const Char*;






  struct StructuredError {
    ptrdiff_t offset_start;
    ptrdiff_t offset_limit;
    String message;
  };




  Reader();




  Reader(const Features& features);















  bool parse(const std::string& document, Value& root,
             bool collectComments = true);

















  bool parse(const char* beginDoc, const char* endDoc, Value& root,
             bool collectComments = true);



  bool parse(IStream& is, Value& root, bool collectComments = true);









  JSONCPP_DEPRECATED("Use getFormattedErrorMessages() instead.")
  String getFormatedErrorMessages() const;








  String getFormattedErrorMessages() const;








  std::vector<StructuredError> getStructuredErrors() const;








  bool pushError(const Value& value, const String& message);









  bool pushError(const Value& value, const String& message, const Value& extra);






  bool good() const;

private:
  enum TokenType {
    tokenEndOfStream = 0,
    tokenObjectBegin,
    tokenObjectEnd,
    tokenArrayBegin,
    tokenArrayEnd,
    tokenString,
    tokenNumber,
    tokenTrue,
    tokenFalse,
    tokenNull,
    tokenArraySeparator,
    tokenMemberSeparator,
    tokenComment,
    tokenError
  };

  class Token {
  public:
    TokenType type_;
    Location start_;
    Location end_;
  };

  class ErrorInfo {
  public:
    Token token_;
    String message_;
    Location extra_;
  };

  using Errors = std::deque<ErrorInfo>;

  bool readToken(Token& token);
  bool readTokenSkippingComments(Token& token);
  void skipSpaces();
  bool match(const Char* pattern, int patternLength);
  bool readComment();
  bool readCStyleComment();
  bool readCppStyleComment();
  bool readString();
  void readNumber();
  bool readValue();
  bool readObject(Token& token);
  bool readArray(Token& token);
  bool decodeNumber(Token& token);
  bool decodeNumber(Token& token, Value& decoded);
  bool decodeString(Token& token);
  bool decodeString(Token& token, String& decoded);
  bool decodeDouble(Token& token);
  bool decodeDouble(Token& token, Value& decoded);
  bool decodeUnicodeCodePoint(Token& token, Location& current, Location end,
                              unsigned int& unicode);
  bool decodeUnicodeEscapeSequence(Token& token, Location& current,
                                   Location end, unsigned int& unicode);
  bool addError(const String& message, Token& token, Location extra = nullptr);
  bool recoverFromError(TokenType skipUntilToken);
  bool addErrorAndRecover(const String& message, Token& token,
                          TokenType skipUntilToken);
  void skipUntilSpace();
  Value& currentValue();
  Char getNextChar();
  void getLocationLineAndColumn(Location location, int& line,
                                int& column) const;
  String getLocationLineAndColumn(Location location) const;
  void addComment(Location begin, Location end, CommentPlacement placement);

  static bool containsNewLine(Location begin, Location end);
  static String normalizeEOL(Location begin, Location end);

  using Nodes = std::stack<Value*>;
  Nodes nodes_;
  Errors errors_;
  String document_;
  Location begin_{};
  Location end_{};
  Location current_{};
  Location lastValueEnd_{};
  Value* lastValue_{};
  String commentsBefore_;
  Features features_;
  bool collectComments_{};
};



class JSON_API CharReader {
public:
  struct JSON_API StructuredError {
    ptrdiff_t offset_start;
    ptrdiff_t offset_limit;
    String message;
  };

  virtual ~CharReader() = default;
















  virtual bool parse(char const* beginDoc, char const* endDoc, Value* root,
                     String* errs);




  std::vector<StructuredError> getStructuredErrors() const;

  class JSON_API Factory {
  public:
    virtual ~Factory() = default;



    virtual CharReader* newCharReader() const = 0;
  };

protected:
  class Impl {
  public:
    virtual ~Impl() = default;
    virtual bool parse(char const* beginDoc, char const* endDoc, Value* root,
                       String* errs) = 0;
    virtual std::vector<StructuredError> getStructuredErrors() const = 0;
  };

  explicit CharReader(std::unique_ptr<Impl> impl) : _impl(std::move(impl)) {}

private:
  std::unique_ptr<Impl> _impl;
};













class JSON_API CharReaderBuilder : public CharReader::Factory {
public:












































  Json::Value settings_;

  CharReaderBuilder();
  ~CharReaderBuilder() override;

  CharReader* newCharReader() const override;




  bool validate(Json::Value* invalid) const;



  Value& operator[](const String& key);






  static void setDefaults(Json::Value* settings);





  static void strictMode(Json::Value* settings);





  static void ecma404Mode(Json::Value* settings);
};





bool JSON_API parseFromStream(CharReader::Factory const&, IStream&, Value* root,
                              String* errs);

























JSON_API IStream& operator>>(IStream&, Value&);

}

#pragma pack(pop)

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif

#endif
