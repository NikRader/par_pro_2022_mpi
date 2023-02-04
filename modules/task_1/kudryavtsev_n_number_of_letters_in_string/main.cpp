// Copyright 2022 Kudryavtsev Nikita
#include <mpi.h>
#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "pch.h"
#include "../../../modules/task_1/kudryavtsev_n_number_of_letters_in_string/number_of_letters_in_string.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Number_Of_Letters_In_String_MPI, Size_1) {
    int rank_pr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_pr);
    int count_elem = 1;
    std::string result;
    if (rank_pr == 0)
        result = CreateSomeString(count_elem);
    int sum = Parallel(result, count_elem);
    if (rank_pr == 0) {
    int result_sum = Sequental(result);
    ASSERT_EQ(result_sum, sum);
    }
}

TEST(Parallel_Number_Of_Letters_In_String_MPI, Size_9) {
    int rank_pr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_pr);
    int count_elem = 9;
    std::string result;
    if (rank_pr == 0)
        result = CreateSomeString(count_elem);
    int sum = Parallel(result, count_elem);
    if (rank_pr == 0) {
        int result_sum = Sequental(result);
        ASSERT_EQ(result_sum, sum);
    }
}

TEST(Parallel_Number_Of_Letters_In_String_MPI, Size_99) {
    int rank_pr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_pr);
    int count_elem = 99;
    std::string result;
    if (rank_pr == 0)
        result = CreateSomeString(count_elem);
    int sum = Parallel(result, count_elem);
    if (rank_pr == 0) {
        int result_sum = Sequental(result);
        ASSERT_EQ(result_sum, sum);
    }
}

TEST(Parallel_Number_Of_Letters_In_String_MPI, Size_999) {
    int rank_pr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_pr);
    int count_elem = 999;
    std::string result;
    if (rank_pr == 0)
        result = CreateSomeString(count_elem);
    int sum = Parallel(result, count_elem);
    if (rank_pr == 0) {
        int result_sum = Sequental(result);
        ASSERT_EQ(result_sum, sum);
    }
}

TEST(Parallel_Number_Of_Letters_In_String_MPI, Size_1400) {
    int rank_pr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_pr);
    int count_elem = 1400;
    std::string result;
    if (rank_pr == 0)
        result = CreateSomeString(count_elem);
    int sum = Parallel(result, count_elem);
    if (rank_pr == 0) {
        int result_sum = Sequental(result);
        ASSERT_EQ(result_sum, sum);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

