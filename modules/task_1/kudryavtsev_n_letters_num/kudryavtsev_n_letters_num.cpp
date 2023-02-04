// Copyright 2022 Kudryavtsev Nikita
#include <mpi.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <string>
#include "../../../modules/task_1/kudryavtsev_n_letters_num/kudryavtsev_n_letters_num.h"


constexpr char base_string[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!,.&^$#@*()-_+=";


int Sequental(std::string result) {
    return  std::count_if(result.begin(), result.end(), [](char c)
        { return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')); });
}

int Parallel(const std::string& result, int elements_count) {
    int count_pr, rank_pr;
    MPI_Comm_size(MPI_COMM_WORLD, &count_pr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_pr);
    int deltaa = elements_count / count_pr;
    int remainder = elements_count - deltaa * count_pr;
    if (rank_pr == 0) {
        for (int num_pr = 1; num_pr < count_pr; num_pr++)
            MPI_Send(&result[remainder + num_pr * deltaa], deltaa, MPI_CHAR,
                num_pr, 0, MPI_COMM_WORLD);
    }
    std::string string_part;
    string_part.resize(rank_pr == 0 ? deltaa + remainder : deltaa);
    if (rank_pr == 0) {
        string_part = { result.begin(), result.begin() + deltaa + remainder };
    } else {
        MPI_Status statuss;
        MPI_Recv(&string_part[0], deltaa, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &statuss);
    }
    int sum = 0;
    int sum_part = Sequental(string_part);
    MPI_Reduce(&sum_part, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return sum;
}

std::string CreateSomeString(int count_elem) {
    std::string result;
    result.resize(count_elem);
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < count_elem; i++)
        result[i] = base_string[rand() % 77];
    return result;
}
