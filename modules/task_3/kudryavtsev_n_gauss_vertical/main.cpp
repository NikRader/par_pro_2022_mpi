// Copyright 2022 Kudryavtsev Nikita
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_3/kudryavtsev_n_gauss_vertical/gauss_vertical.h"


TEST(Gauss_Vertical_MPI, Test_height_priority_1) {
  int Rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  int* picture = nullptr;
  const int w = 100, h = 200;

  if (Rank == 0) {
    picture = GetPictureRandom(w, h);
  }
  double begin = MPI_Wtime();
  int* new_picture_parallel = Parallel(picture, w, h);
  double finish = MPI_Wtime();

  if (Rank == 0) {
    double time_one = finish - begin;
    begin = MPI_Wtime();
    int* reference_new_picture = Sequential(picture, w, h);
    finish = MPI_Wtime();
    double time_two = finish - begin;
    std::cout << time_one << " " << time_two << " " << time_two / time_one << std::endl;
    for (int i = 0; i < (w - 2) * (h - 2); i += (w - 2)) {
      ASSERT_EQ(new_picture_parallel[i], reference_new_picture[i]);
    }
  }
}

TEST(Gauss_Vertical_MPI, Test_width_priority_1) {
  int Rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  int* picture = nullptr;
  const int w = 300, h = 100;

  if (Rank == 0) {
    picture = GetPictureRandom(w, h);
  }
  double begin = MPI_Wtime();
  int* new_picture_parallel = Parallel(picture, w, h);
  double finish = MPI_Wtime();

  if (Rank == 0) {
    double time_one = finish - begin;
    begin = MPI_Wtime();
    int* reference_new_picture = Sequential(picture, w, h);
    finish = MPI_Wtime();
    double time2 = finish - begin;
    std::cout << time_one << " " << time2 << " " << time2 / time_one << std::endl;
    for (int i = 0; i < (w - 2) * (h - 2); i += (w - 2)) {
      ASSERT_EQ(new_picture_parallel[i], reference_new_picture[i]);
    }
  }
}

TEST(Gauss_Vertical_MPI, Test_equal) {
  int Rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  int* picture = nullptr;
  const int w = 500, h = 500;

  if (Rank == 0) {
    picture = GetPictureRandom(w, h);
  }
  double begin = MPI_Wtime();
  int* new_picture_parallel = Parallel(picture, w, h);
  double finish = MPI_Wtime();

  if (Rank == 0) {
    double time_one = finish - begin;
    begin = MPI_Wtime();
    int* reference_new_picture = Sequential(picture, w, h);
    finish = MPI_Wtime();
    double time2 = finish - begin;
    std::cout << time_one << " " << time2 << " " << time2 / time_one << std::endl;
    for (int i = 0; i < (w - 2) * (h - 2); i += (w - 2)) {
      ASSERT_EQ(new_picture_parallel[i], reference_new_picture[i]);
    }
  }
}

TEST(Gauss_Vertical_MPI, Test_height_priority_2) {
  int Rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  int* picture = nullptr;
  const int w = 512, h = 1123;

  if (Rank == 0) {
    picture = GetPictureRandom(w, h);
  }
  double begin = MPI_Wtime();
  int* new_picture_parallel = Parallel(picture, w, h);
  double finish = MPI_Wtime();

  if (Rank == 0) {
    double time_one = finish - begin;
    begin = MPI_Wtime();
    int* reference_new_picture = Sequential(picture, w, h);
    finish = MPI_Wtime();
    double time_two = finish - begin;
    std::cout << time_one << " " << time_two << " " << time_two / time_one << std::endl;
    for (int i = 0; i < (w - 2) * (h - 2); i += (w - 2)) {
      ASSERT_EQ(new_picture_parallel[i], reference_new_picture[i]);
    }
  }
}

TEST(Gauss_Vertical_MPI, Test_width_priority_2) {
  int Rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  int* picture = nullptr;
  const int w = 1230, h = 512;

  if (Rank == 0) {
    picture = GetPictureRandom(w, h);
  }
  double begin = MPI_Wtime();
  int* new_picture_parallel = Parallel(picture, w, h);
  double finish = MPI_Wtime();

  if (Rank == 0) {
    double time1 = finish - begin;
    begin = MPI_Wtime();
    int* reference_new_picture = Sequential(picture, w, h);
    finish = MPI_Wtime();
    double time2 = finish - begin;
    std::cout << time1 << " " << time2 << " " << time2 / time1 << std::endl;
    for (int i = 0; i < (w - 2) * (h - 2); i += (w - 2)) {
      ASSERT_EQ(new_picture_parallel[i], reference_new_picture[i]);
    }
  }
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}

