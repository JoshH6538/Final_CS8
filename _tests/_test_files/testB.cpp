#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>

#include "../../includes/sql/sql.h"
using namespace std;

const vector<string> command_list = {

//****************************************************************************
// CREATE AND INSERT
//****************************************************************************
//---- employee table ----------
"make table employee fields last, first, dep",
"insert into employee values Blow, Joe, CS",
"insert into employee values Johnson, \"Jimmy\", Chemistry",
"insert into employee values Yang, Bo, CS",
"insert into employee values \"Jackson\", Billy, Math",
"insert into employee values Johnson, Billy, \"Phys Ed\"",
"insert into employee values \"Van Gogh\", \"Jim Bob\", \"Phys Ed\"",
"select * from employee",
//----- student table ----------
"make table student fields fname, lname, major, age",
"insert into student values Flo, Yao, CS, 20",
"insert into student values \"Flo\", \"Jackson\", Math, 21",
"insert into student values Calvin, Woo, Physics, 22",
"insert into student values \"Anna Grace\", \"Del Rio\", CS, 22",
"select * from student",
//****************************************************************************
// SIMPLE SELECT
//****************************************************************************
"select * from student",
//------- simple strings -------------------
"select * from student where lname = Jackson",
//----- quoted strings ---------------------
"select * from student where lname = \"Del Rio\"",
//-------- non-existing string ------------------
"select * from student where lname = \"Does Not Exist\"",
//****************************************************************************
// RELATIONAL OPERATORS:
//****************************************************************************
//.................
//:Greater Than :
//.................
"select * from student where lname > Yang",
//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
"select * from student where age > 50",
//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
"select * from student where age > 53",
//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
"select * from student where age > 54",
//.................
//:Greater Equal :
//.................
"select * from student where lname >= Yang",
//. . . . . . (Greater Equal non-existing: ) . . . . . . . . . . .
"select * from employee where last >= Jack",
//.................
//:Less Than :
//.................
//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
"select * from student where lname < Jackson",
//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
"select * from student where age < 20",
//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .
"select * from student where age < 19",
//.................
//:Less Equal :
//.................
"select * from student where lname <= Smith",
//. . . . . . (Less Equal non-existing: ) . . . . . . . . . . .
"select * from employee where last <= Peach",
//****************************************************************************
// LOGICAL OPERATORS
//****************************************************************************
//.................
//:AND :
//.................
"select * from student where fname = \"Flo\" and lname = \"Yao\"",
//.................
//:OR :
//.................
"select * from student where fname = Flo or lname = Jackson",
//.................
//:OR AND :
//.................
"select * from student where fname = Flo or major = CS and age <= 23",
//.................
//:AND OR AND :
//.................
"select * from student where age <30 and major=CS or major = Physics and lname = Jackson",
//.................
//:OR AND OR :
//.................
"select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson"

};

const int TABLE_COMMANDS = 34;

bool test_stub(bool debug = false)
{
  SQL sql;
  Table t;

  for (int i = 0; i < 7; i++){
    cout << ">" << command_list[i] << endl;
    sql.command(command_list[i]);
  }

  cout << "\n>" << command_list[7] << endl;
  if (debug)
        cout<< sql.command(command_list[7])<<endl;
  else
        t = sql.command(command_list[7]);
  cout << "basic_test: records selected: "<<sql.select_recnos() << endl;

  for (int i = 8; i < 13; i++){
    cout << ">" << command_list[i] << endl;
    sql.command(command_list[i]);
  }

  for (int i = 13; i < TABLE_COMMANDS; i++){
    cout << "\n>" << command_list[i] << endl;
    if (debug)
          cout<< sql.command(command_list[i])<<endl;
    else
          t = sql.command(command_list[i]);
    cout << "basic_test: records selected: "<<sql.select_recnos() << endl;
  }

  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(true));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}
