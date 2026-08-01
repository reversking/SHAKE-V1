




#ifndef JSON_WRITER_H_INCLUDED
#define JSON_WRITER_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "value.h"
#endif
#include <ostream>
#include <string>
#include <vector>



#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING) && defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#pragma pack(push)
#pragma pack()

namespace Json {

class Value;













class JSON_API StreamWriter {
protected:
  OStream* sout_;
public:
  StreamWriter();
  virtual ~StreamWriter();







  virtual int write(Value const& root, OStream* sout) = 0;



  class JSON_API Factory {
  public:
    virtual ~Factory();



    virtual StreamWriter* newStreamWriter() const = 0;
  };
};




String JSON_API writeString(StreamWriter::Factory const& factory,
                            Value const& root);
















class JSON_API StreamWriterBuilder : public StreamWriter::Factory {
public:






























  Json::Value settings_;

  StreamWriterBuilder();
  ~StreamWriterBuilder() override;




  StreamWriter* newStreamWriter() const override;




  bool validate(Json::Value* invalid) const;


  Value& operator[](const String& key);






  static void setDefaults(Json::Value* settings);
};




class JSON_API Writer {
public:
  virtual ~Writer();

  virtual String write(const Value& root) = 0;
};










#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
class JSON_API FastWriter : public Writer {
public:
  FastWriter();
  ~FastWriter() override = default;

  void enableYAMLCompatibility();






  void dropNullPlaceholders();

  void omitEndingLineFeed();

public:
  String write(const Value& root) override;

private:
  void writeValue(const Value& value);

  String document_;
  bool yamlCompatibilityEnabled_{false};
  bool dropNullPlaceholders_{false};
  bool omitEndingLineFeed_{false};
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

























#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
class JSON_API StyledWriter : public Writer {
public:
  StyledWriter();
  ~StyledWriter() override = default;

public:




  String write(const Value& root) override;

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultilineArray(const Value& value);
  void pushValue(const String& value);
  void writeIndent();
  void writeWithIndent(const String& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  static bool hasCommentForValue(const Value& value);
  static String normalizeEOL(const String& text);

  using ChildValues = std::vector<String>;

  ChildValues childValues_;
  String document_;
  String indentString_;
  unsigned int rightMargin_{74};
  unsigned int indentSize_{3};
  bool addChildValues_{false};
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif


























#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
class JSON_API StyledStreamWriter {
public:



  StyledStreamWriter(String indentation = "\t");
  ~StyledStreamWriter() = default;

public:






  void write(OStream& out, const Value& root);

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultilineArray(const Value& value);
  void pushValue(const String& value);
  void writeIndent();
  void writeWithIndent(const String& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  static bool hasCommentForValue(const Value& value);
  static String normalizeEOL(const String& text);

  using ChildValues = std::vector<String>;

  ChildValues childValues_;
  OStream* document_;
  String indentString_;
  unsigned int rightMargin_{74};
  String indentation_;
  bool addChildValues_ : 1;
  bool indented_ : 1;
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(JSON_HAS_INT64)
String JSON_API valueToString(Int value);
String JSON_API valueToString(UInt value);
#endif
String JSON_API valueToString(LargestInt value);
String JSON_API valueToString(LargestUInt value);
String JSON_API valueToString(
    double value, unsigned int precision = Value::defaultRealPrecision,
    PrecisionType precisionType = PrecisionType::significantDigits);
String JSON_API valueToString(bool value);
String JSON_API valueToQuotedString(const char* value);
String JSON_API valueToQuotedString(const char* value, size_t length);



JSON_API OStream& operator<<(OStream&, const Value& root);

}

#pragma pack(pop)

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif

#endif
