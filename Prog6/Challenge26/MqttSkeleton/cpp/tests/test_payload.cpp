// Project and standard headers must come BEFORE CppUTest/TestHarness.h.
#include <string>

#include "payload.hpp"

#include <CppUTest/TestHarness.h>

using sim::extractNumber;
using sim::extractString;
using sim::Payload;

TEST_GROUP(Payload){};

TEST(Payload, EmptyPayloadIsEmptyObject) {
    STRCMP_EQUAL("{}", Payload().str().c_str());
}

TEST(Payload, WritesFieldsInOrderWithJsonTypes) {
    const std::string json = Payload()
                                 .add("timestamp", static_cast<int64_t>(1801234567))
                                 .add("value", 37.2)
                                 .add("unit", "C")
                                 .add("connected", true)
                                 .str();
    STRCMP_EQUAL(R"({"timestamp":1801234567,"value":37.2,"unit":"C","connected":true})", json.c_str());
}

TEST(Payload, EscapesQuotesInStrings) {
    STRCMP_EQUAL(R"({"note":"say \"hi\""})", Payload().add("note", "say \"hi\"").str().c_str());
}

TEST(Payload, ExtractsNumbersAndStrings) {
    const std::string json = R"({"timestamp": 1801234567, "value": 38.5, "unit":"C"})";
    DOUBLES_EQUAL(38.5, *extractNumber(json, "value"), 1e-9);
    DOUBLES_EQUAL(1801234567.0, *extractNumber(json, "timestamp"), 1e-9);
    STRCMP_EQUAL("C", extractString(json, "unit")->c_str());
}

TEST(Payload, MissingOrWrongTypeGivesNullopt) {
    const std::string json = R"({"value":"not a number","unit":"C"})";
    CHECK_FALSE(extractNumber(json, "value").has_value());
    CHECK_FALSE(extractNumber(json, "missing").has_value());
    CHECK_FALSE(extractString(json, "missing").has_value());
    CHECK_FALSE(extractString(R"({"value":37.2})", "value").has_value());
}
