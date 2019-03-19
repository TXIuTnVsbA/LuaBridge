// https://github.com/vinniefalco/LuaBridge
//
// Copyright 2018, Dmitry Tarakanov
// SPDX-License-Identifier: MIT


#include "TestBase.h"
#include "TestTypes.h"

#include "LuaBridge/List.h"

#include <list>

template <class T>
struct ListTest : TestBase
{
};

TYPED_TEST_CASE_P (ListTest);

TYPED_TEST_P (ListTest, LuaRef)
{
  using Traits = TypeTraits <TypeParam>;

  this->runLua ("result = {" + Traits::list () + "}");

  std::list <TypeParam> expected (Traits::VALUES, Traits::VALUES + 3);
  std::list <TypeParam> actual = this->result ();
  ASSERT_EQ (expected, actual);
}

REGISTER_TYPED_TEST_CASE_P (ListTest, LuaRef);

INSTANTIATE_TYPED_TEST_CASE_P(ListTest, ListTest, TestTypes);


struct ListTests : TestBase
{
};


TEST_F (ListTests, PassToFunction)
{
  runLua (
    "function foo (list) "
    "  result = list "
    "end");

  auto foo = luabridge::getGlobal (L, "foo");

  resetResult ();

  std::list <int> lvalue {10, 20, 30};
  foo (lvalue);
  ASSERT_TRUE (result ().isTable ());
  ASSERT_EQ (lvalue, result ().cast <std::list<int>> ());

  resetResult ();

  const std::list <int> constLvalue = lvalue;
  foo (constLvalue);
  ASSERT_TRUE (result ().isTable ());
  ASSERT_EQ (lvalue, result ().cast <std::list<int>> ());
}
