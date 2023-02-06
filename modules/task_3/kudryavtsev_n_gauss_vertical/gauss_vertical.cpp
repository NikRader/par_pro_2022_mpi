// Copyright 2022 Kudryavtsev Nikita

#include "../../../modules/task_3/kudryavtsev_n_gauss_vertical/gauss_vertical.h"
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <random>

int* GetPictureRandom(int w, int h) {
  std::mt19937 generate(time(0));
  std::uniform_int_distribution<> UID(0, 255);
  int* picture = new int[w * h];
  for (int i = 0; i < w * h; i++) {
    picture[i] = UID(generate);
  }
  return picture;
}

int Clamp_pict(int val, int max_val, int min_val) {
  if (val > max_val) return max_val;
  if (val < min_val) return min_val;
  return val;
}

float* GetKern() {
  int Size = 3;
  int Len = 9;

  float* Kern = new float[Len];
  float delta = 3.f;
  float normal = 0;
  int radius_signed = static_cast<int>(1);

  for (int X = -radius_signed; X <= radius_signed; X++)
    for (int Y = -radius_signed; Y <= radius_signed; Y++) {
      std::size_t ID_X = (X + 1) * Size + (Y + 1);
      Kern[ID_X] = std::exp(-(X * X + Y * Y) / (delta * delta));
      normal += Kern[ID_X];
    }
  for (int i = 0; i < Len; i++) {
    Kern[i] /= normal;
  }

  return Kern;
}

float GetNewColor (const int* picture, int X, int Y, int w, int h, const float* Kern) {
  float result = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int ID_X = (i + 1) * 3 + j + 1;
      int pix = picture[X + j + (Y + i) * w];
      result += pix * Kern[ID_X];
    }
  }
  return Clamp_pict(result, 255, 0);
}

int* Sequential(const int* picture, int w, int h) {
  int* new_picture = new int[(w - 2) * (h - 2)];
  const float* Kern = GetKern();
  for (int i = 1; i < h - 1; i++) {
    for (int j = 1; j < w - 1; j++) {
      new_picture[j - 1 + (i - 1) * (w - 2)] = GetNewColor(picture, j, i, w, h, Kern);        
    }
  }
  return new_picture;
}

int* Parallel(const int* picture, int w, int h) {
  int Size, Rank;
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  int Delt = (w - 2) / Size;
  int Remain = (w - 2) % Size;

  if (Delt) {
    int* Counts = new int[Size];
    int *Displs = new int[Size];
    for (int i = 0; i < Size; i++) {
      if (i == 0) {
        Counts[i] = Delt + Remain + 2;
        Displs[i] = 0;
      } else {
        Counts[i] = Delt + 2;
        Displs[i] = Delt * i + Remain;
      }
    }

    int* local_matr;

    int tmp = Delt;
    if (Rank == 0) tmp += Remain;

    local_matr = new int[(tmp + 2) * h];

    for (int i = 0; i < h; i++) {
      MPI_Scatterv(picture + i * w, Counts, Displs, MPI_INT,
                   local_matr + i * (tmp + 2), tmp + 2, MPI_INT, 0,
                   MPI_COMM_WORLD);
    }

    local_matr = Sequential(local_matr, tmp + 2, h);

    int* global_matr = nullptr;

    for (int i = 0; i < Size; i++) {
      Counts[i] -= 2;
    }

    if (Rank == 0) global_matr = new int[(w - 2) * (h - 2)];

    for (int i = 0; i < h - 2; i++) {
      MPI_Gatherv(local_matr + i * tmp, tmp, MPI_INT,
          global_matr + i * (w - 2), Counts, Displs, MPI_INT, 0, MPI_COMM_WORLD);                  
    }

    return global_matr;
  } else {
    if (Rank == 0) {
      return Sequential(picture, w, h);
    } else {
      return nullptr;
    }
  }
}





