#ifndef FORECAST_TEMPERATURE_H
#define FORECAST_TEMPERATURE_H




    // !!! This file is generated using emlearn !!!

    #include <stdint.h>
    

static inline float forecast_temperature_tree_0(const float *features, int32_t features_length) {
          if (features[6] < 33.626952f) {
              if (features[22] < 6.405860f) {
                  if (features[25] < -0.098311f) {
                      if (features[2] < -0.354529f) {
                          if (features[25] < -0.704635f) {
                              return -0.310800f;
                          } else {
                              return -0.809735f;
                          }
                      } else {
                          if (features[13] < 10.342388f) {
                              return -0.109489f;
                          } else {
                              return 0.310486f;
                          }
                      }
                  } else {
                      if (features[26] < -0.058926f) {
                          if (features[16] < 3.989314f) {
                              return 0.093711f;
                          } else {
                              return 0.299985f;
                          }
                      } else {
                          if (features[23] < 0.089482f) {
                              return -0.063364f;
                          } else {
                              return -0.005189f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.816708f) {
                      return -0.590513f;
                  } else {
                      if (features[26] < -0.530488f) {
                          if (features[4] < 1.094122f) {
                              return 0.701486f;
                          } else {
                              return -0.182727f;
                          }
                      } else {
                          return 1.764583f;
                      }
                  }
              }
          } else {
              if (features[22] < 9.334332f) {
                  if (features[25] < -0.625065f) {
                      if (features[14] < 60.813768f) {
                          return -0.590476f;
                      } else {
                          if (features[19] < -0.330988f) {
                              return -2.004444f;
                          } else {
                              return -0.605517f;
                          }
                      }
                  } else {
                      if (features[22] < 7.891746f) {
                          if (features[21] < 0.131732f) {
                              return -0.031167f;
                          } else {
                              return 0.480952f;
                          }
                      } else {
                          return -0.197143f;
                      }
                  }
              } else {
                  return -1.392007f;
              }
          }
        }
        

static inline float forecast_temperature_tree_1(const float *features, int32_t features_length) {
          if (features[25] < -0.104729f) {
              if (features[1] < -0.390353f) {
                  return -0.917059f;
              } else {
                  if (features[16] < 5.467466f) {
                      if (features[21] < -1.171003f) {
                          if (features[18] < -0.525599f) {
                              return -0.465137f;
                          } else {
                              return -0.153919f;
                          }
                      } else {
                          if (features[25] < -0.584663f) {
                              return -0.057364f;
                          } else {
                              return -0.152889f;
                          }
                      }
                  } else {
                      if (features[20] < -0.525960f) {
                          if (features[22] < 7.009006f) {
                              return -0.424286f;
                          } else {
                              return -1.417778f;
                          }
                      } else {
                          if (features[26] < -0.820633f) {
                              return 0.138387f;
                          } else {
                              return -0.591707f;
                          }
                      }
                  }
              }
          } else {
              if (features[26] < 0.047132f) {
                  if (features[12] < -4.773559f) {
                      if (features[0] < 33.323294f) {
                          if (features[4] < 1.110243f) {
                              return 2.445000f;
                          } else {
                              return 1.373889f;
                          }
                      } else {
                          return -0.271739f;
                      }
                  } else {
                      if (features[6] < 35.122055f) {
                          if (features[16] < 7.727040f) {
                              return 0.202044f;
                          } else {
                              return 0.848462f;
                          }
                      } else {
                          return -1.857232f;
                      }
                  }
              } else {
                  if (features[6] < 25.723367f) {
                      return 0.132632f;
                  } else {
                      if (features[3] < -0.065282f) {
                          if (features[20] < -1.219658f) {
                              return 0.083158f;
                          } else {
                              return -0.041906f;
                          }
                      } else {
                          if (features[25] < 0.066368f) {
                              return -0.176995f;
                          } else {
                              return -0.071805f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_2(const float *features, int32_t features_length) {
          if (features[25] < -0.139243f) {
              if (features[5] < -1.482728f) {
                  if (features[6] < 31.467868f) {
                      if (features[16] < 3.128932f) {
                          return -0.405000f;
                      } else {
                          if (features[3] < -0.134386f) {
                              return -0.058819f;
                          } else {
                              return 0.269028f;
                          }
                      }
                  } else {
                      return -1.191200f;
                  }
              } else {
                  if (features[2] < -0.260226f) {
                      if (features[6] < 27.826184f) {
                          return 0.140909f;
                      } else {
                          if (features[26] < -0.477072f) {
                              return -0.698958f;
                          } else {
                              return -0.508485f;
                          }
                      }
                  } else {
                      if (features[1] < -0.001844f) {
                          if (features[1] < -0.097948f) {
                              return -0.235278f;
                          } else {
                              return -0.115499f;
                          }
                      } else {
                          if (features[25] < -0.402409f) {
                              return -0.086177f;
                          } else {
                              return 0.009374f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 2.974346f) {
                  if (features[20] < 1.299231f) {
                      if (features[2] < 0.207622f) {
                          if (features[5] < 0.081956f) {
                              return -0.043964f;
                          } else {
                              return -0.100811f;
                          }
                      } else {
                          return -0.215517f;
                      }
                  } else {
                      if (features[0] < 27.857696f) {
                          return 0.152105f;
                      } else {
                          return 0.061724f;
                      }
                  }
              } else {
                  if (features[24] < 0.129685f) {
                      if (features[12] < -5.727057f) {
                          if (features[6] < 32.281740f) {
                              return 2.040417f;
                          } else {
                              return -0.235625f;
                          }
                      } else {
                          if (features[5] < -0.533871f) {
                              return 0.721538f;
                          } else {
                              return 0.208677f;
                          }
                      }
                  } else {
                      return -1.365513f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_3(const float *features, int32_t features_length) {
          if (features[25] < -0.417438f) {
              if (features[6] < 32.447047f) {
                  if (features[0] < 28.654720f) {
                      if (features[25] < -0.962341f) {
                          if (features[0] < 27.360709f) {
                              return 0.026471f;
                          } else {
                              return 0.417451f;
                          }
                      } else {
                          if (features[5] < -0.079435f) {
                              return -0.026751f;
                          } else {
                              return -0.202808f;
                          }
                      }
                  } else {
                      if (features[3] < -0.198118f) {
                          if (features[19] < -0.151199f) {
                              return -0.427764f;
                          } else {
                              return -0.228587f;
                          }
                      } else {
                          if (features[22] < 1.660633f) {
                              return -0.199398f;
                          } else {
                              return -0.085012f;
                          }
                      }
                  }
              } else {
                  if (features[14] < 68.469782f) {
                      if (features[7] < 0.179946f) {
                          if (features[0] < 35.374610f) {
                              return -0.110836f;
                          } else {
                              return -0.437222f;
                          }
                      } else {
                          if (features[0] < 35.395865f) {
                              return -0.648043f;
                          } else {
                              return -0.107273f;
                          }
                      }
                  } else {
                      return -1.457549f;
                  }
              }
          } else {
              if (features[13] < -5.174506f) {
                  if (features[6] < 33.598732f) {
                      if (features[22] < 5.303335f) {
                          if (features[22] < 4.723006f) {
                              return 0.259375f;
                          } else {
                              return 0.435286f;
                          }
                      } else {
                          if (features[5] < 1.189100f) {
                              return 2.573000f;
                          } else {
                              return 0.804595f;
                          }
                      }
                  } else {
                      return -0.568473f;
                  }
              } else {
                  if (features[22] < 2.502246f) {
                      if (features[21] < 0.738850f) {
                          if (features[4] < 0.333017f) {
                              return -0.057934f;
                          } else {
                              return -0.221176f;
                          }
                      } else {
                          if (features[15] < 0.505800f) {
                              return 0.101176f;
                          } else {
                              return -0.031250f;
                          }
                      }
                  } else {
                      if (features[0] < 34.721569f) {
                          if (features[21] < 0.602886f) {
                              return 0.062520f;
                          } else {
                              return 0.266323f;
                          }
                      } else {
                          if (features[9] < -0.267921f) {
                              return -0.494500f;
                          } else {
                              return -0.272174f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_4(const float *features, int32_t features_length) {
          if (features[6] < 33.328155f) {
              if (features[22] < 4.289455f) {
                  if (features[25] < 0.042313f) {
                      if (features[25] < -0.857580f) {
                          if (features[0] < 29.385772f) {
                              return 0.061893f;
                          } else {
                              return -0.110139f;
                          }
                      } else {
                          if (features[12] < 2.726423f) {
                              return -0.114633f;
                          } else {
                              return -0.311100f;
                          }
                      }
                  } else {
                      if (features[22] < 2.916636f) {
                          if (features[20] < 0.322876f) {
                              return -0.053861f;
                          } else {
                              return 0.076544f;
                          }
                      } else {
                          if (features[25] < 0.197631f) {
                              return -0.050000f;
                          } else {
                              return 0.143050f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.170320f) {
                      if (features[20] < 0.134048f) {
                          if (features[11] < 1.556637f) {
                              return -0.208509f;
                          } else {
                              return -0.626086f;
                          }
                      } else {
                          if (features[21] < 0.149535f) {
                              return 0.338687f;
                          } else {
                              return -0.001828f;
                          }
                      }
                  } else {
                      if (features[16] < 7.773661f) {
                          if (features[26] < -0.826262f) {
                              return 0.136207f;
                          } else {
                              return 0.452005f;
                          }
                      } else {
                          if (features[0] < 31.853690f) {
                              return 2.049259f;
                          } else {
                              return 0.474839f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 7.313076f) {
                  if (features[26] < -0.775616f) {
                      if (features[21] < -0.135608f) {
                          return -0.168438f;
                      } else {
                          if (features[23] < 0.034485f) {
                              return -0.022368f;
                          } else {
                              return 0.314706f;
                          }
                      }
                  } else {
                      return -1.055517f;
                  }
              } else {
                  if (features[25] < -0.105090f) {
                      if (features[25] < -0.652968f) {
                          if (features[2] < -0.232753f) {
                              return -1.026667f;
                          } else {
                              return -0.557727f;
                          }
                      } else {
                          if (features[13] < -2.552282f) {
                              return -0.842000f;
                          } else {
                              return -0.058889f;
                          }
                      }
                  } else {
                      return -1.214821f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_5(const float *features, int32_t features_length) {
          if (features[25] < 0.579987f) {
              if (features[3] < -0.462447f) {
                  if (features[0] < 31.691326f) {
                      if (features[13] < 4.000268f) {
                          return -0.056461f;
                      } else {
                          return -0.467593f;
                      }
                  } else {
                      if (features[0] < 33.218367f) {
                          return -0.427625f;
                      } else {
                          return -1.080267f;
                      }
                  }
              } else {
                  if (features[22] < 9.392794f) {
                      if (features[12] < 3.185052f) {
                          if (features[16] < 5.490781f) {
                              return -0.065774f;
                          } else {
                              return 0.079301f;
                          }
                      } else {
                          if (features[14] < 78.029857f) {
                              return -0.388696f;
                          } else {
                              return -0.120248f;
                          }
                      }
                  } else {
                      return -1.285556f;
                  }
              }
          } else {
              if (features[15] < 1.365853f) {
                  if (features[16] < 3.273444f) {
                      if (features[23] < 0.135288f) {
                          if (features[6] < 25.980855f) {
                              return 0.052121f;
                          } else {
                              return -0.072275f;
                          }
                      } else {
                          if (features[5] < -0.173478f) {
                              return 0.144444f;
                          } else {
                              return 0.040242f;
                          }
                      }
                  } else {
                      if (features[14] < 64.407159f) {
                          return -0.612956f;
                      } else {
                          if (features[22] < 5.207857f) {
                              return 0.226210f;
                          } else {
                              return 0.509036f;
                          }
                      }
                  }
              } else {
                  if (features[14] < 76.273173f) {
                      return 0.755561f;
                  } else {
                      if (features[5] < 1.630338f) {
                          return 1.991739f;
                      } else {
                          return 1.237333f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_6(const float *features, int32_t features_length) {
          if (features[13] < -7.038508f) {
              if (features[25] < 0.832368f) {
                  if (features[0] < 32.930790f) {
                      return 1.078571f;
                  } else {
                      if (features[13] < -16.937307f) {
                          return 0.428667f;
                      } else {
                          return -0.826786f;
                      }
                  }
              } else {
                  return 1.932857f;
              }
          } else {
              if (features[25] < 0.736914f) {
                  if (features[19] < 0.230028f) {
                      if (features[1] < -0.140118f) {
                          if (features[6] < 32.040546f) {
                              return -0.239065f;
                          } else {
                              return -0.871743f;
                          }
                      } else {
                          if (features[25] < -0.410083f) {
                              return -0.139295f;
                          } else {
                              return -0.014650f;
                          }
                      }
                  } else {
                      if (features[16] < 3.798373f) {
                          if (features[26] < 0.387173f) {
                              return -0.089099f;
                          } else {
                              return -0.033743f;
                          }
                      } else {
                          if (features[25] < 0.001604f) {
                              return 0.065282f;
                          } else {
                              return 0.382407f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 5.332661f) {
                      if (features[26] < -0.005532f) {
                          if (features[22] < 4.572475f) {
                              return 0.150781f;
                          } else {
                              return 0.498584f;
                          }
                      } else {
                          if (features[3] < -0.205729f) {
                              return 0.081892f;
                          } else {
                              return -0.055249f;
                          }
                      }
                  } else {
                      if (features[13] < -3.537571f) {
                          return 1.227500f;
                      } else {
                          return 0.469583f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_7(const float *features, int32_t features_length) {
          if (features[16] < 3.805134f) {
              if (features[21] < -1.080285f) {
                  if (features[7] < 0.137502f) {
                      if (features[22] < 3.467604f) {
                          if (features[23] < 0.323478f) {
                              return -0.073439f;
                          } else {
                              return 0.018206f;
                          }
                      } else {
                          return -0.561875f;
                      }
                  } else {
                      if (features[13] < 3.375588f) {
                          if (features[7] < 0.187891f) {
                              return -0.165909f;
                          } else {
                              return -0.522367f;
                          }
                      } else {
                          if (features[8] < 86.154232f) {
                              return -0.948125f;
                          } else {
                              return -0.347000f;
                          }
                      }
                  }
              } else {
                  if (features[26] < 0.024123f) {
                      if (features[13] < 3.734816f) {
                          if (features[26] < -0.414283f) {
                              return -0.050385f;
                          } else {
                              return 0.090041f;
                          }
                      } else {
                          if (features[25] < 0.257075f) {
                              return -0.357200f;
                          } else {
                              return 0.190000f;
                          }
                      }
                  } else {
                      if (features[6] < 27.929838f) {
                          if (features[5] < 0.050772f) {
                              return -0.009872f;
                          } else {
                              return -0.123080f;
                          }
                      } else {
                          if (features[22] < 3.147395f) {
                              return -0.102381f;
                          } else {
                              return -0.016264f;
                          }
                      }
                  }
              }
          } else {
              if (features[25] < -0.712485f) {
                  if (features[16] < 4.735165f) {
                      if (features[18] < 0.048022f) {
                          if (features[26] < 0.221904f) {
                              return -0.054354f;
                          } else {
                              return -0.177500f;
                          }
                      } else {
                          return 0.104643f;
                      }
                  } else {
                      if (features[4] < -1.040506f) {
                          return -1.445088f;
                      } else {
                          if (features[5] < -1.172854f) {
                              return -0.252857f;
                          } else {
                              return -0.686923f;
                          }
                      }
                  }
              } else {
                  if (features[25] < 0.402101f) {
                      if (features[25] < -0.433277f) {
                          if (features[16] < 8.915229f) {
                              return -0.158724f;
                          } else {
                              return -0.646842f;
                          }
                      } else {
                          if (features[25] < -0.045170f) {
                              return -0.056865f;
                          } else {
                              return 0.202661f;
                          }
                      }
                  } else {
                      if (features[12] < -6.980703f) {
                          if (features[25] < 0.790476f) {
                              return 0.427059f;
                          } else {
                              return 2.532000f;
                          }
                      } else {
                          if (features[14] < 57.639592f) {
                              return -1.492388f;
                          } else {
                              return 0.329816f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_8(const float *features, int32_t features_length) {
          if (features[21] < 0.348449f) {
              if (features[8] < 55.371576f) {
                  return -1.258286f;
              } else {
                  if (features[11] < 1.089779f) {
                      if (features[20] < 0.249194f) {
                          if (features[2] < -0.338757f) {
                              return -0.546296f;
                          } else {
                              return -0.063701f;
                          }
                      } else {
                          if (features[6] < 31.389404f) {
                              return 0.051611f;
                          } else {
                              return 0.223621f;
                          }
                      }
                  } else {
                      if (features[22] < 6.862382f) {
                          if (features[19] < -0.902044f) {
                              return -0.559492f;
                          } else {
                              return -0.114366f;
                          }
                      } else {
                          if (features[26] < -0.725961f) {
                              return 0.398194f;
                          } else {
                              return -1.101078f;
                          }
                      }
                  }
              }
          } else {
              if (features[12] < -7.562220f) {
                  return 1.161034f;
              } else {
                  if (features[6] < 32.969624f) {
                      if (features[15] < 1.643654f) {
                          if (features[15] < 1.367258f) {
                              return 0.190177f;
                          } else {
                              return 0.714762f;
                          }
                      } else {
                          return 0.910212f;
                      }
                  } else {
                      if (features[23] < 0.164365f) {
                          return -0.300870f;
                      } else {
                          return -0.633661f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_9(const float *features, int32_t features_length) {
          if (features[0] < 33.402540f) {
              if (features[16] < 8.124132f) {
                  if (features[25] < -0.388096f) {
                      if (features[4] < -1.140113f) {
                          if (features[8] < 87.303095f) {
                              return -0.842963f;
                          } else {
                              return -0.496000f;
                          }
                      } else {
                          if (features[6] < 28.031375f) {
                              return -0.035666f;
                          } else {
                              return -0.150335f;
                          }
                      }
                  } else {
                      if (features[26] < 0.102618f) {
                          if (features[26] < -0.904796f) {
                              return 0.043868f;
                          } else {
                              return 0.242590f;
                          }
                      } else {
                          if (features[4] < 0.226237f) {
                              return -0.053402f;
                          } else {
                              return -0.180980f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -1.300544f) {
                      if (features[5] < 1.843563f) {
                          return 2.226111f;
                      } else {
                          return 1.017143f;
                      }
                  } else {
                      return 0.339231f;
                  }
              }
          } else {
              if (features[22] < 9.491357f) {
                  if (features[14] < 64.904377f) {
                      if (features[26] < -0.710694f) {
                          if (features[26] < -0.813030f) {
                              return 0.070353f;
                          } else {
                              return -0.246471f;
                          }
                      } else {
                          return -0.438636f;
                      }
                  } else {
                      if (features[25] < -0.474729f) {
                          if (features[16] < 7.968566f) {
                              return -1.634792f;
                          } else {
                              return -0.628750f;
                          }
                      } else {
                          return 0.268667f;
                      }
                  }
              } else {
                  if (features[21] < 0.360151f) {
                      return -1.865048f;
                  } else {
                      return -0.728000f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_10(const float *features, int32_t features_length) {
          if (features[13] < -4.270098f) {
              if (features[0] < 33.089346f) {
                  if (features[16] < 6.118869f) {
                      if (features[25] < 0.554157f) {
                          if (features[8] < 80.522659f) {
                              return 0.064767f;
                          } else {
                              return -0.290625f;
                          }
                      } else {
                          if (features[16] < 5.521314f) {
                              return 0.283905f;
                          } else {
                              return 1.875625f;
                          }
                      }
                  } else {
                      if (features[12] < -5.938215f) {
                          return 2.094762f;
                      } else {
                          return 0.706400f;
                      }
                  }
              } else {
                  if (features[22] < 9.298590f) {
                      return 0.160476f;
                  } else {
                      return -0.819852f;
                  }
              }
          } else {
              if (features[25] < -0.331121f) {
                  if (features[22] < 6.036328f) {
                      if (features[25] < -0.889356f) {
                          if (features[0] < 29.951889f) {
                              return 0.049660f;
                          } else {
                              return -0.102184f;
                          }
                      } else {
                          if (features[7] < 0.354474f) {
                              return -0.150807f;
                          } else {
                              return -0.509825f;
                          }
                      }
                  } else {
                      if (features[3] < -0.202393f) {
                          if (features[16] < 7.617435f) {
                              return -1.107111f;
                          } else {
                              return -0.627350f;
                          }
                      } else {
                          if (features[25] < -0.699592f) {
                              return -0.560000f;
                          } else {
                              return 0.062195f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 3.834419f) {
                      if (features[21] < 0.618067f) {
                          if (features[22] < 3.708462f) {
                              return -0.057001f;
                          } else {
                              return 0.133333f;
                          }
                      } else {
                          if (features[14] < 67.468327f) {
                              return -0.049524f;
                          } else {
                              return 0.142584f;
                          }
                      }
                  } else {
                      if (features[25] < 0.191217f) {
                          if (features[22] < 5.595461f) {
                              return -0.145956f;
                          } else {
                              return 0.110429f;
                          }
                      } else {
                          if (features[0] < 33.138361f) {
                              return 0.290967f;
                          } else {
                              return -0.821176f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_11(const float *features, int32_t features_length) {
          if (features[0] < 33.121323f) {
              if (features[16] < 3.714516f) {
                  if (features[13] < 3.409837f) {
                      if (features[26] < 0.548016f) {
                          if (features[6] < 28.101968f) {
                              return 0.011188f;
                          } else {
                              return -0.038261f;
                          }
                      } else {
                          if (features[23] < 0.421761f) {
                              return -0.084367f;
                          } else {
                              return 0.023530f;
                          }
                      }
                  } else {
                      if (features[5] < -1.120296f) {
                          return -0.715000f;
                      } else {
                          if (features[0] < 28.701880f) {
                              return -0.007451f;
                          } else {
                              return -0.210884f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -5.156268f) {
                      if (features[22] < 7.836335f) {
                          if (features[1] < 0.084545f) {
                              return 0.368109f;
                          } else {
                              return 1.480000f;
                          }
                      } else {
                          return 1.667586f;
                      }
                  } else {
                      if (features[21] < -0.651148f) {
                          if (features[17] < -0.198321f) {
                              return -0.401098f;
                          } else {
                              return -0.059955f;
                          }
                      } else {
                          if (features[12] < 5.695831f) {
                              return 0.165279f;
                          } else {
                              return -0.352074f;
                          }
                      }
                  }
              }
          } else {
              if (features[22] < 9.517512f) {
                  if (features[25] < -0.688006f) {
                      if (features[8] < 61.769490f) {
                          if (features[0] < 33.858184f) {
                              return -0.120588f;
                          } else {
                              return -0.626667f;
                          }
                      } else {
                          if (features[8] < 66.133994f) {
                              return -0.492083f;
                          } else {
                              return -1.295867f;
                          }
                      }
                  } else {
                      if (features[6] < 33.413537f) {
                          return 0.481364f;
                      } else {
                          if (features[8] < 64.397523f) {
                              return 0.014412f;
                          } else {
                              return -0.468667f;
                          }
                      }
                  }
              } else {
                  return -1.162524f;
              }
          }
        }
        

static inline float forecast_temperature_tree_12(const float *features, int32_t features_length) {
          if (features[25] < 0.830692f) {
              if (features[24] < 0.204763f) {
                  if (features[25] < 0.634862f) {
                      if (features[25] < 0.082699f) {
                          if (features[1] < -0.238632f) {
                              return -0.595203f;
                          } else {
                              return -0.118808f;
                          }
                      } else {
                          if (features[12] < -4.013024f) {
                              return -0.544286f;
                          } else {
                              return 0.086962f;
                          }
                      }
                  } else {
                      if (features[26] < -0.010299f) {
                          if (features[12] < -1.766873f) {
                              return 0.838939f;
                          } else {
                              return 0.346813f;
                          }
                      } else {
                          if (features[0] < 27.226697f) {
                              return 0.014054f;
                          } else {
                              return -0.125686f;
                          }
                      }
                  }
              } else {
                  return -1.480548f;
              }
          } else {
              if (features[11] < -2.037857f) {
                  if (features[22] < 6.609480f) {
                      return 0.201579f;
                  } else {
                      return 2.066250f;
                  }
              } else {
                  if (features[16] < 4.316147f) {
                      if (features[16] < 2.728298f) {
                          if (features[21] < 0.711872f) {
                              return -0.049651f;
                          } else {
                              return 0.095286f;
                          }
                      } else {
                          if (features[26] < -0.124776f) {
                              return 0.153613f;
                          } else {
                              return 0.021250f;
                          }
                      }
                  } else {
                      if (features[16] < 5.642469f) {
                          if (features[4] < 0.123150f) {
                              return 0.420240f;
                          } else {
                              return 0.119580f;
                          }
                      } else {
                          return 0.783333f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_13(const float *features, int32_t features_length) {
          if (features[25] < 0.353330f) {
              if (features[12] < 2.332019f) {
                  if (features[25] < -0.071263f) {
                      if (features[22] < 7.016880f) {
                          if (features[20] < 0.120767f) {
                              return -0.113105f;
                          } else {
                              return 0.007516f;
                          }
                      } else {
                          if (features[20] < -0.270768f) {
                              return -0.945000f;
                          } else {
                              return -0.380882f;
                          }
                      }
                  } else {
                      if (features[18] < 0.511059f) {
                          if (features[0] < 34.469858f) {
                              return 0.030527f;
                          } else {
                              return -0.565217f;
                          }
                      } else {
                          if (features[25] < 0.179285f) {
                              return 0.533125f;
                          } else {
                              return 0.791634f;
                          }
                      }
                  }
              } else {
                  if (features[6] < 32.201394f) {
                      if (features[26] < -0.630747f) {
                          if (features[6] < 29.653132f) {
                              return -0.190054f;
                          } else {
                              return -0.732400f;
                          }
                      } else {
                          if (features[17] < -0.110848f) {
                              return -0.285778f;
                          } else {
                              return -0.056211f;
                          }
                      }
                  } else {
                      if (features[8] < 70.471559f) {
                          if (features[25] < -0.769786f) {
                              return -0.301364f;
                          } else {
                              return 0.390000f;
                          }
                      } else {
                          return -1.168750f;
                      }
                  }
              }
          } else {
              if (features[24] < 0.246505f) {
                  if (features[16] < 4.177609f) {
                      if (features[16] < 1.627052f) {
                          if (features[23] < 0.324385f) {
                              return -0.065332f;
                          } else {
                              return 0.113333f;
                          }
                      } else {
                          if (features[21] < 0.715899f) {
                              return -0.007144f;
                          } else {
                              return 0.141553f;
                          }
                      }
                  } else {
                      if (features[13] < -10.386450f) {
                          if (features[25] < 0.824742f) {
                              return 0.458571f;
                          } else {
                              return 2.433750f;
                          }
                      } else {
                          if (features[16] < 4.671498f) {
                              return 0.129018f;
                          } else {
                              return 0.467629f;
                          }
                      }
                  }
              } else {
                  return -1.480548f;
              }
          }
        }
        

static inline float forecast_temperature_tree_14(const float *features, int32_t features_length) {
          if (features[25] < 0.758297f) {
              if (features[6] < 33.318857f) {
                  if (features[25] < -0.322744f) {
                      if (features[3] < -0.134303f) {
                          if (features[6] < 29.265096f) {
                              return -0.021785f;
                          } else {
                              return -0.332391f;
                          }
                      } else {
                          if (features[26] < 0.059988f) {
                              return -0.035202f;
                          } else {
                              return -0.119079f;
                          }
                      }
                  } else {
                      if (features[16] < 5.356267f) {
                          if (features[21] < 0.572975f) {
                              return -0.054911f;
                          } else {
                              return 0.271721f;
                          }
                      } else {
                          if (features[4] < -0.358360f) {
                              return 2.091333f;
                          } else {
                              return 0.242635f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 7.916968f) {
                      if (features[26] < -0.606341f) {
                          if (features[26] < -0.834302f) {
                              return 0.104321f;
                          } else {
                              return -0.355074f;
                          }
                      } else {
                          return -1.291067f;
                      }
                  } else {
                      if (features[0] < 33.789481f) {
                          return -0.172500f;
                      } else {
                          if (features[25] < 0.153612f) {
                              return -0.673000f;
                          } else {
                              return -2.095714f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 5.669857f) {
                  if (features[26] < -0.550880f) {
                      if (features[14] < 94.391459f) {
                          if (features[21] < 0.755280f) {
                              return 0.275338f;
                          } else {
                              return 0.413333f;
                          }
                      } else {
                          return 0.812000f;
                      }
                  } else {
                      if (features[26] < -0.016916f) {
                          if (features[4] < 0.261196f) {
                              return 0.175906f;
                          } else {
                              return 0.065616f;
                          }
                      } else {
                          if (features[12] < 1.441215f) {
                              return -0.049600f;
                          } else {
                              return 0.110588f;
                          }
                      }
                  }
              } else {
                  if (features[26] < -0.273940f) {
                      return 2.079355f;
                  } else {
                      return 0.526875f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_15(const float *features, int32_t features_length) {
          if (features[25] < -0.188859f) {
              if (features[6] < 33.632105f) {
                  if (features[1] < -0.209495f) {
                      if (features[7] < 0.177865f) {
                          return 0.025852f;
                      } else {
                          if (features[25] < -0.737227f) {
                              return -0.504444f;
                          } else {
                              return -0.844963f;
                          }
                      }
                  } else {
                      if (features[20] < -0.338974f) {
                          if (features[13] < 3.768285f) {
                              return -0.109097f;
                          } else {
                              return -0.364926f;
                          }
                      } else {
                          if (features[22] < 7.236805f) {
                              return -0.081168f;
                          } else {
                              return 0.325882f;
                          }
                      }
                  }
              } else {
                  if (features[23] < 0.223093f) {
                      if (features[14] < 63.507334f) {
                          if (features[6] < 36.027217f) {
                              return 0.166667f;
                          } else {
                              return -0.369333f;
                          }
                      } else {
                          if (features[2] < 0.009018f) {
                              return -0.394444f;
                          } else {
                              return -0.652000f;
                          }
                      }
                  } else {
                      return -1.288395f;
                  }
              }
          } else {
              if (features[24] < 0.165660f) {
                  if (features[13] < -11.623409f) {
                      if (features[26] < -0.710061f) {
                          return 0.224737f;
                      } else {
                          return 1.977391f;
                      }
                  } else {
                      if (features[16] < 1.581014f) {
                          if (features[21] < 0.684068f) {
                              return -0.061879f;
                          } else {
                              return 0.110500f;
                          }
                      } else {
                          if (features[25] < 0.061876f) {
                              return -0.077553f;
                          } else {
                              return 0.201039f;
                          }
                      }
                  }
              } else {
                  return -1.365513f;
              }
          }
        }
        

static inline float forecast_temperature_tree_16(const float *features, int32_t features_length) {
          if (features[25] < -0.039630f) {
              if (features[3] < -0.497341f) {
                  if (features[0] < 29.525264f) {
                      return -0.323946f;
                  } else {
                      if (features[15] < 1.576568f) {
                          return -1.101067f;
                      } else {
                          return -0.627879f;
                      }
                  }
              } else {
                  if (features[3] < -0.234250f) {
                      if (features[0] < 29.348362f) {
                          if (features[19] < -0.350718f) {
                              return -0.233182f;
                          } else {
                              return -0.037523f;
                          }
                      } else {
                          if (features[26] < -0.423466f) {
                              return -0.619242f;
                          } else {
                              return -0.294053f;
                          }
                      }
                  } else {
                      if (features[25] < -0.864241f) {
                          if (features[22] < 5.614322f) {
                              return -0.016852f;
                          } else {
                              return -0.500000f;
                          }
                      } else {
                          if (features[5] < 0.387126f) {
                              return -0.096379f;
                          } else {
                              return -0.305976f;
                          }
                      }
                  }
              }
          } else {
              if (features[22] < 3.205728f) {
                  if (features[26] < -0.083163f) {
                      if (features[0] < 29.249267f) {
                          if (features[21] < 2.280185f) {
                              return 0.070000f;
                          } else {
                              return 0.172500f;
                          }
                      } else {
                          if (features[14] < 69.874242f) {
                              return 0.287778f;
                          } else {
                              return 0.085000f;
                          }
                      }
                  } else {
                      if (features[18] < 0.309655f) {
                          if (features[5] < 0.161662f) {
                              return -0.049288f;
                          } else {
                              return -0.177692f;
                          }
                      } else {
                          if (features[14] < 70.146523f) {
                              return -0.103810f;
                          } else {
                              return 0.094167f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 32.232074f) {
                      if (features[13] < -10.111811f) {
                          return 2.230000f;
                      } else {
                          if (features[13] < -5.378572f) {
                              return 0.988738f;
                          } else {
                              return 0.242797f;
                          }
                      }
                  } else {
                      if (features[24] < 0.105012f) {
                          if (features[14] < 70.038564f) {
                              return 0.193204f;
                          } else {
                              return -0.195652f;
                          }
                      } else {
                          return -1.365513f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_17(const float *features, int32_t features_length) {
          if (features[25] < -0.093686f) {
              if (features[16] < 5.930372f) {
                  if (features[1] < -0.291296f) {
                      return -0.710432f;
                  } else {
                      if (features[26] < -0.841017f) {
                          if (features[1] < -0.134626f) {
                              return -0.767895f;
                          } else {
                              return -0.175222f;
                          }
                      } else {
                          if (features[13] < 3.165629f) {
                              return -0.065395f;
                          } else {
                              return -0.189475f;
                          }
                      }
                  }
              } else {
                  if (features[26] < -0.802500f) {
                      if (features[5] < 0.293516f) {
                          if (features[13] < 0.573365f) {
                              return 0.129500f;
                          } else {
                              return 0.975000f;
                          }
                      } else {
                          if (features[16] < 8.936609f) {
                              return 0.165000f;
                          } else {
                              return -0.778125f;
                          }
                      }
                  } else {
                      if (features[17] < -0.147426f) {
                          return -1.221429f;
                      } else {
                          if (features[14] < 70.418078f) {
                              return -0.547358f;
                          } else {
                              return -0.990267f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 4.125990f) {
                  if (features[26] < -0.056899f) {
                      if (features[20] < -0.148573f) {
                          if (features[25] < 0.684965f) {
                              return -0.199017f;
                          } else {
                              return 0.034815f;
                          }
                      } else {
                          if (features[26] < -0.727931f) {
                              return -0.010000f;
                          } else {
                              return 0.156250f;
                          }
                      }
                  } else {
                      if (features[26] < 0.393590f) {
                          if (features[25] < 0.998733f) {
                              return -0.039574f;
                          } else {
                              return 0.072143f;
                          }
                      } else {
                          if (features[3] < -0.053664f) {
                              return -0.028639f;
                          } else {
                              return -0.090616f;
                          }
                      }
                  }
              } else {
                  if (features[24] < 0.230532f) {
                      if (features[15] < 1.863637f) {
                          if (features[26] < -0.830443f) {
                              return 0.194484f;
                          } else {
                              return 0.397719f;
                          }
                      } else {
                          if (features[6] < 31.060160f) {
                              return 2.137891f;
                          } else {
                              return 0.098529f;
                          }
                      }
                  } else {
                      return -1.480548f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_18(const float *features, int32_t features_length) {
          if (features[13] < -6.315639f) {
              if (features[6] < 33.884838f) {
                  if (features[13] < -7.708821f) {
                      if (features[26] < -0.687491f) {
                          return 0.543158f;
                      } else {
                          if (features[4] < 1.086860f) {
                              return 1.990625f;
                          } else {
                              return 1.549333f;
                          }
                      }
                  } else {
                      if (features[22] < 4.920736f) {
                          return 0.043333f;
                      } else {
                          return 0.862941f;
                      }
                  }
              } else {
                  return -0.376758f;
              }
          } else {
              if (features[25] < 0.217020f) {
                  if (features[13] < 4.703155f) {
                      if (features[2] < -0.479806f) {
                          return -0.585556f;
                      } else {
                          if (features[0] < 33.392301f) {
                              return -0.063574f;
                          } else {
                              return -0.251985f;
                          }
                      }
                  } else {
                      if (features[1] < -0.142126f) {
                          if (features[16] < 5.922930f) {
                              return -0.505898f;
                          } else {
                              return -1.468444f;
                          }
                      } else {
                          if (features[21] < -0.334534f) {
                              return -0.353587f;
                          } else {
                              return -0.022464f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 2.248692f) {
                      if (features[20] < 0.566158f) {
                          if (features[20] < -0.469519f) {
                              return 0.092222f;
                          } else {
                              return -0.065671f;
                          }
                      } else {
                          if (features[8] < 74.599885f) {
                              return -0.023889f;
                          } else {
                              return 0.122812f;
                          }
                      }
                  } else {
                      if (features[19] < -0.671789f) {
                          return -0.207589f;
                      } else {
                          if (features[0] < 31.951014f) {
                              return 0.215573f;
                          } else {
                              return -0.119766f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_19(const float *features, int32_t features_length) {
          if (features[12] < -6.886513f) {
              if (features[26] < -0.634825f) {
                  return 0.096667f;
              } else {
                  return 2.406875f;
              }
          } else {
              if (features[21] < 0.041696f) {
                  if (features[9] < 2.461171f) {
                      if (features[20] < 0.200758f) {
                          if (features[6] < 33.304555f) {
                              return -0.080653f;
                          } else {
                              return -0.494444f;
                          }
                      } else {
                          if (features[0] < 32.229914f) {
                              return 0.029167f;
                          } else {
                              return 0.378571f;
                          }
                      }
                  } else {
                      return -0.932173f;
                  }
              } else {
                  if (features[25] < -0.279933f) {
                      if (features[16] < 8.708398f) {
                          if (features[25] < -0.468858f) {
                              return -0.053152f;
                          } else {
                              return 0.191833f;
                          }
                      } else {
                          return -0.495789f;
                      }
                  } else {
                      if (features[25] < -0.074460f) {
                          if (features[1] < -0.068416f) {
                              return -0.574375f;
                          } else {
                              return -0.285385f;
                          }
                      } else {
                          if (features[0] < 34.734501f) {
                              return 0.235625f;
                          } else {
                              return -0.603482f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_20(const float *features, int32_t features_length) {
          if (features[25] < 0.186426f) {
              if (features[3] < -0.289116f) {
                  if (features[0] < 28.422678f) {
                      if (features[23] < 0.078482f) {
                          return 0.142500f;
                      } else {
                          if (features[14] < 91.046539f) {
                              return -0.129140f;
                          } else {
                              return -0.030455f;
                          }
                      }
                  } else {
                      if (features[0] < 33.288134f) {
                          if (features[10] < 2.008170f) {
                              return -0.348377f;
                          } else {
                              return -0.726135f;
                          }
                      } else {
                          if (features[23] < 0.318644f) {
                              return -0.551818f;
                          } else {
                              return -1.763778f;
                          }
                      }
                  }
              } else {
                  if (features[25] < 0.063930f) {
                      if (features[0] < 34.454226f) {
                          if (features[16] < 7.492019f) {
                              return -0.094604f;
                          } else {
                              return 0.206667f;
                          }
                      } else {
                          if (features[22] < 7.323909f) {
                              return 0.025000f;
                          } else {
                              return -0.837500f;
                          }
                      }
                  } else {
                      if (features[26] < -0.868339f) {
                          if (features[0] < 30.670770f) {
                              return 0.197500f;
                          } else {
                              return 0.064242f;
                          }
                      } else {
                          if (features[25] < 0.129593f) {
                              return -0.068758f;
                          } else {
                              return 0.034474f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 4.681740f) {
                  if (features[16] < 0.790747f) {
                      if (features[6] < 25.735568f) {
                          return 0.134000f;
                      } else {
                          if (features[20] < -0.409902f) {
                              return 0.116667f;
                          } else {
                              return -0.069917f;
                          }
                      }
                  } else {
                      if (features[21] < 0.896211f) {
                          if (features[26] < -0.186330f) {
                              return 0.085578f;
                          } else {
                              return -0.028740f;
                          }
                      } else {
                          if (features[23] < 0.760809f) {
                              return 0.164018f;
                          } else {
                              return -0.010000f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 33.913126f) {
                      if (features[15] < 1.148771f) {
                          if (features[14] < 97.256047f) {
                              return 0.343829f;
                          } else {
                              return 1.027124f;
                          }
                      } else {
                          if (features[12] < -4.398423f) {
                              return 1.773611f;
                          } else {
                              return 0.665161f;
                          }
                      }
                  } else {
                      if (features[16] < 6.314139f) {
                          return 0.136667f;
                      } else {
                          return -1.389805f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_21(const float *features, int32_t features_length) {
          if (features[19] < 0.180589f) {
              if (features[24] < 0.272951f) {
                  if (features[25] < -0.469625f) {
                      if (features[22] < 6.593950f) {
                          if (features[17] < -0.243818f) {
                              return -0.345729f;
                          } else {
                              return -0.111710f;
                          }
                      } else {
                          if (features[23] < 0.196879f) {
                              return -0.491091f;
                          } else {
                              return -1.466026f;
                          }
                      }
                  } else {
                      if (features[16] < 4.355706f) {
                          if (features[25] < 0.747730f) {
                              return -0.076981f;
                          } else {
                              return -0.000940f;
                          }
                      } else {
                          if (features[25] < 0.699905f) {
                              return 0.060776f;
                          } else {
                              return 0.568608f;
                          }
                      }
                  }
              } else {
                  return -1.480548f;
              }
          } else {
              if (features[16] < 5.696213f) {
                  if (features[16] < 3.044260f) {
                      if (features[20] < 0.337693f) {
                          if (features[20] < 0.003607f) {
                              return -0.250048f;
                          } else {
                              return -0.036420f;
                          }
                      } else {
                          if (features[2] < -0.025810f) {
                              return 0.102000f;
                          } else {
                              return -0.000417f;
                          }
                      }
                  } else {
                      if (features[25] < 0.150126f) {
                          if (features[3] < 0.049321f) {
                              return 0.032333f;
                          } else {
                              return -0.103737f;
                          }
                      } else {
                          if (features[8] < 97.888856f) {
                              return 0.229187f;
                          } else {
                              return 0.771345f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 33.072985f) {
                      if (features[1] < 0.268005f) {
                          if (features[23] < 0.404828f) {
                              return 0.835636f;
                          } else {
                              return 0.403366f;
                          }
                      } else {
                          return 1.316667f;
                      }
                  } else {
                      return -0.154167f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_22(const float *features, int32_t features_length) {
          if (features[20] < 0.058396f) {
              if (features[6] < 33.842829f) {
                  if (features[25] < 0.099582f) {
                      if (features[2] < -0.406075f) {
                          if (features[17] < -0.036099f) {
                              return -0.729877f;
                          } else {
                              return -0.302000f;
                          }
                      } else {
                          if (features[13] < 2.060787f) {
                              return -0.093632f;
                          } else {
                              return -0.204865f;
                          }
                      }
                  } else {
                      if (features[22] < 4.458367f) {
                          if (features[16] < 2.206663f) {
                              return -0.056744f;
                          } else {
                              return 0.003192f;
                          }
                      } else {
                          if (features[22] < 6.695492f) {
                              return 0.232349f;
                          } else {
                              return 1.293333f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 9.190080f) {
                      if (features[26] < -0.684967f) {
                          if (features[25] < -0.408907f) {
                              return -0.524286f;
                          } else {
                              return -0.034595f;
                          }
                      } else {
                          if (features[25] < -0.868101f) {
                              return -0.695000f;
                          } else {
                              return -1.415417f;
                          }
                      }
                  } else {
                      return -1.631176f;
                  }
              }
          } else {
              if (features[15] < 1.011213f) {
                  if (features[26] < -0.077266f) {
                      if (features[25] < 0.548450f) {
                          if (features[25] < 0.253049f) {
                              return 0.053840f;
                          } else {
                              return 0.255926f;
                          }
                      } else {
                          if (features[25] < 0.663773f) {
                              return 0.413093f;
                          } else {
                              return 0.226641f;
                          }
                      }
                  } else {
                      if (features[0] < 30.986968f) {
                          if (features[26] < 0.733370f) {
                              return 0.038128f;
                          } else {
                              return -0.084561f;
                          }
                      } else {
                          if (features[0] < 31.783385f) {
                              return -0.139804f;
                          } else {
                              return -0.232500f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -7.565191f) {
                      if (features[0] < 31.062481f) {
                          return 2.866000f;
                      } else {
                          if (features[0] < 33.343837f) {
                              return 1.252941f;
                          } else {
                              return 0.083125f;
                          }
                      }
                  } else {
                      if (features[25] < 0.436450f) {
                          if (features[15] < 1.795803f) {
                              return 0.148683f;
                          } else {
                              return -0.264889f;
                          }
                      } else {
                          if (features[20] < 0.759915f) {
                              return 0.264494f;
                          } else {
                              return 0.657083f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_23(const float *features, int32_t features_length) {
          if (features[24] < 0.102065f) {
              if (features[25] < -0.530610f) {
                  if (features[16] < 4.692570f) {
                      if (features[25] < -0.918822f) {
                          if (features[26] < -0.235602f) {
                              return 0.128066f;
                          } else {
                              return -0.072288f;
                          }
                      } else {
                          if (features[23] < 0.572629f) {
                              return -0.131509f;
                          } else {
                              return -0.960335f;
                          }
                      }
                  } else {
                      if (features[6] < 31.356078f) {
                          return -0.061190f;
                      } else {
                          if (features[23] < 0.321085f) {
                              return -0.519255f;
                          } else {
                              return -1.177742f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -8.101618f) {
                      if (features[26] < -0.540354f) {
                          if (features[22] < 8.779718f) {
                              return 0.837778f;
                          } else {
                              return -0.251500f;
                          }
                      } else {
                          return 2.481875f;
                      }
                  } else {
                      if (features[13] < -5.375268f) {
                          if (features[8] < 72.665850f) {
                              return 0.224786f;
                          } else {
                              return 0.950968f;
                          }
                      } else {
                          if (features[22] < 3.692231f) {
                              return -0.032261f;
                          } else {
                              return 0.142556f;
                          }
                      }
                  }
              }
          } else {
              return -1.365513f;
          }
        }
        

static inline float forecast_temperature_tree_24(const float *features, int32_t features_length) {
          if (features[25] < 0.090221f) {
              if (features[12] < 4.062826f) {
                  if (features[6] < 27.012730f) {
                      if (features[12] < 0.852374f) {
                          if (features[0] < 26.308013f) {
                              return 0.097813f;
                          } else {
                              return -0.058060f;
                          }
                      } else {
                          if (features[15] < 0.488325f) {
                              return -0.016889f;
                          } else {
                              return 0.168649f;
                          }
                      }
                  } else {
                      if (features[7] < 0.488675f) {
                          if (features[18] < 0.185579f) {
                              return -0.148386f;
                          } else {
                              return 0.031356f;
                          }
                      } else {
                          return -0.607949f;
                      }
                  }
              } else {
                  if (features[6] < 32.509203f) {
                      if (features[9] < 1.191448f) {
                          if (features[26] < -0.573408f) {
                              return -0.318296f;
                          } else {
                              return -0.080341f;
                          }
                      } else {
                          return -0.524733f;
                      }
                  } else {
                      return -0.680000f;
                  }
              }
          } else {
              if (features[22] < 3.372719f) {
                  if (features[19] < 0.314006f) {
                      if (features[21] < 1.712961f) {
                          if (features[18] < 0.001939f) {
                              return -0.057042f;
                          } else {
                              return 0.015116f;
                          }
                      } else {
                          return 0.213750f;
                      }
                  } else {
                      if (features[14] < 74.278989f) {
                          if (features[3] < -0.035964f) {
                              return 0.079615f;
                          } else {
                              return -0.032333f;
                          }
                      } else {
                          if (features[5] < -0.110290f) {
                              return 0.156364f;
                          } else {
                              return 0.090000f;
                          }
                      }
                  }
              } else {
                  if (features[24] < 0.196057f) {
                      if (features[13] < -7.918542f) {
                          if (features[6] < 32.609166f) {
                              return 1.577105f;
                          } else {
                              return -0.220500f;
                          }
                      } else {
                          if (features[16] < 3.857769f) {
                              return 0.048339f;
                          } else {
                              return 0.300405f;
                          }
                      }
                  } else {
                      return -1.480548f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_25(const float *features, int32_t features_length) {
          if (features[25] < -0.163211f) {
              if (features[22] < 6.823426f) {
                  if (features[12] < 3.107853f) {
                      if (features[18] < -0.442966f) {
                          if (features[22] < 4.873257f) {
                              return -0.165814f;
                          } else {
                              return -0.407444f;
                          }
                      } else {
                          if (features[0] < 27.143775f) {
                              return 0.000967f;
                          } else {
                              return -0.092560f;
                          }
                      }
                  } else {
                      if (features[20] < -0.525480f) {
                          if (features[10] < 1.182264f) {
                              return -0.250000f;
                          } else {
                              return -0.566313f;
                          }
                      } else {
                          if (features[14] < 77.046397f) {
                              return -0.398313f;
                          } else {
                              return -0.082599f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.549734f) {
                      if (features[23] < 0.301285f) {
                          if (features[21] < -0.477785f) {
                              return -0.842692f;
                          } else {
                              return -0.460588f;
                          }
                      } else {
                          return -1.469841f;
                      }
                  } else {
                      return 0.329583f;
                  }
              }
          } else {
              if (features[14] < 56.772540f) {
                  return -0.906394f;
              } else {
                  if (features[13] < -5.393431f) {
                      if (features[6] < 33.747876f) {
                          if (features[26] < -0.724425f) {
                              return 0.226508f;
                          } else {
                              return 1.388197f;
                          }
                      } else {
                          return -0.235789f;
                      }
                  } else {
                      if (features[22] < 3.451045f) {
                          if (features[26] < 0.430316f) {
                              return 0.036519f;
                          } else {
                              return -0.070176f;
                          }
                      } else {
                          if (features[18] < 0.002939f) {
                              return 0.112817f;
                          } else {
                              return 0.273900f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_26(const float *features, int32_t features_length) {
          if (features[25] < 0.252041f) {
              if (features[1] < -0.198502f) {
                  if (features[26] < 0.125931f) {
                      if (features[19] < -0.613098f) {
                          return -0.939242f;
                      } else {
                          if (features[8] < 78.892471f) {
                              return -0.573704f;
                          } else {
                              return -0.363182f;
                          }
                      }
                  } else {
                      return 0.080758f;
                  }
              } else {
                  if (features[0] < 33.487878f) {
                      if (features[25] < 0.014624f) {
                          if (features[21] < -1.591012f) {
                              return -0.273860f;
                          } else {
                              return -0.088343f;
                          }
                      } else {
                          if (features[26] < 0.223181f) {
                              return 0.208447f;
                          } else {
                              return -0.025278f;
                          }
                      }
                  } else {
                      if (features[18] < -0.186793f) {
                          return -1.006842f;
                      } else {
                          if (features[25] < -0.666483f) {
                              return -0.704074f;
                          } else {
                              return -0.051316f;
                          }
                      }
                  }
              }
          } else {
              if (features[26] < -0.060360f) {
                  if (features[13] < -10.864548f) {
                      if (features[0] < 33.891779f) {
                          return 1.707857f;
                      } else {
                          return 0.188889f;
                      }
                  } else {
                      if (features[6] < 32.879399f) {
                          if (features[11] < -2.706220f) {
                              return 0.866990f;
                          } else {
                              return 0.251622f;
                          }
                      } else {
                          return -0.712352f;
                      }
                  }
              } else {
                  if (features[4] < -0.317077f) {
                      return 0.080000f;
                  } else {
                      if (features[20] < 1.696429f) {
                          if (features[4] < 0.093114f) {
                              return -0.048723f;
                          } else {
                              return -0.125000f;
                          }
                      } else {
                          if (features[8] < 86.389454f) {
                              return -0.021875f;
                          } else {
                              return 0.102273f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_27(const float *features, int32_t features_length) {
          if (features[13] < -11.519462f) {
              if (features[26] < -0.698132f) {
                  return 0.290476f;
              } else {
                  return 1.840400f;
              }
          } else {
              if (features[25] < -0.562940f) {
                  if (features[4] < -0.938170f) {
                      if (features[18] < -0.398923f) {
                          return -1.166061f;
                      } else {
                          if (features[8] < 89.590051f) {
                              return -0.714561f;
                          } else {
                              return -0.256667f;
                          }
                      }
                  } else {
                      if (features[16] < 6.515210f) {
                          if (features[21] < -1.161431f) {
                              return -0.264038f;
                          } else {
                              return -0.069224f;
                          }
                      } else {
                          if (features[5] < -0.619228f) {
                              return -0.466136f;
                          } else {
                              return -0.981818f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 9.923319f) {
                      if (features[22] < 4.605839f) {
                          if (features[25] < 0.447078f) {
                              return -0.074439f;
                          } else {
                              return 0.016622f;
                          }
                      } else {
                          if (features[26] < -0.665546f) {
                              return 0.133422f;
                          } else {
                              return 0.752503f;
                          }
                      }
                  } else {
                      return -1.429206f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_28(const float *features, int32_t features_length) {
          if (features[25] < -0.384746f) {
              if (features[16] < 5.294329f) {
                  if (features[25] < -0.886699f) {
                      if (features[18] < -0.859925f) {
                          return -0.342857f;
                      } else {
                          if (features[16] < 0.173353f) {
                              return -0.289231f;
                          } else {
                              return -0.003973f;
                          }
                      }
                  } else {
                      if (features[21] < -1.536293f) {
                          return -0.672688f;
                      } else {
                          if (features[0] < 27.759844f) {
                              return -0.079454f;
                          } else {
                              return -0.177728f;
                          }
                      }
                  }
              } else {
                  if (features[21] < -1.074073f) {
                      return -1.260595f;
                  } else {
                      if (features[14] < 64.887106f) {
                          if (features[25] < -0.627841f) {
                              return -0.469310f;
                          } else {
                              return 0.274000f;
                          }
                      } else {
                          if (features[15] < 0.834993f) {
                              return -0.474146f;
                          } else {
                              return -0.674118f;
                          }
                      }
                  }
              }
          } else {
              if (features[0] < 35.104337f) {
                  if (features[22] < 4.591370f) {
                      if (features[26] < 0.587152f) {
                          if (features[21] < 1.320108f) {
                              return 0.006580f;
                          } else {
                              return 0.154095f;
                          }
                      } else {
                          if (features[5] < -0.362722f) {
                              return 0.018690f;
                          } else {
                              return -0.073605f;
                          }
                      }
                  } else {
                      if (features[25] < 0.655579f) {
                          if (features[25] < 0.002466f) {
                              return 0.033126f;
                          } else {
                              return 0.239442f;
                          }
                      } else {
                          if (features[15] < 2.794544f) {
                              return 0.692999f;
                          } else {
                              return 1.497727f;
                          }
                      }
                  }
              } else {
                  return -1.441224f;
              }
          }
        }
        

static inline float forecast_temperature_tree_29(const float *features, int32_t features_length) {
          if (features[25] < 0.254743f) {
              if (features[3] < -0.280839f) {
                  if (features[6] < 29.250581f) {
                      if (features[8] < 87.567443f) {
                          return 0.063580f;
                      } else {
                          if (features[25] < -0.787804f) {
                              return 0.100476f;
                          } else {
                              return -0.162500f;
                          }
                      }
                  } else {
                      if (features[26] < 0.276415f) {
                          if (features[7] < 0.594323f) {
                              return -0.819770f;
                          } else {
                              return 0.008039f;
                          }
                      } else {
                          return -0.159798f;
                      }
                  }
              } else {
                  if (features[12] < 3.095376f) {
                      if (features[4] < 1.377714f) {
                          if (features[25] < 0.080459f) {
                              return -0.079293f;
                          } else {
                              return 0.084035f;
                          }
                      } else {
                          return -0.758000f;
                      }
                  } else {
                      if (features[14] < 83.233662f) {
                          if (features[21] < -0.843211f) {
                              return -0.476471f;
                          } else {
                              return -0.233389f;
                          }
                      } else {
                          if (features[11] < 2.129013f) {
                              return -0.188673f;
                          } else {
                              return -0.048028f;
                          }
                      }
                  }
              }
          } else {
              if (features[13] < -9.506957f) {
                  if (features[26] < -0.617189f) {
                      return 0.515172f;
                  } else {
                      return 2.117368f;
                  }
              } else {
                  if (features[6] < 33.161706f) {
                      if (features[16] < 3.701883f) {
                          if (features[21] < 1.392399f) {
                              return -0.048291f;
                          } else {
                              return 0.123701f;
                          }
                      } else {
                          if (features[16] < 5.883689f) {
                              return 0.281471f;
                          } else {
                              return 0.494083f;
                          }
                      }
                  } else {
                      return -0.886085f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_30(const float *features, int32_t features_length) {
          if (features[25] < -0.095894f) {
              if (features[7] < 0.203955f) {
                  if (features[19] < -0.846858f) {
                      if (features[26] < 0.291500f) {
                          if (features[11] < 0.640291f) {
                              return -0.231275f;
                          } else {
                              return -0.504138f;
                          }
                      } else {
                          return -0.021388f;
                      }
                  } else {
                      if (features[6] < 34.911575f) {
                          if (features[1] < -0.104638f) {
                              return -0.194939f;
                          } else {
                              return -0.072699f;
                          }
                      } else {
                          return -0.408000f;
                      }
                  }
              } else {
                  if (features[21] < -0.703996f) {
                      if (features[14] < 76.291985f) {
                          if (features[6] < 33.251795f) {
                              return -0.673750f;
                          } else {
                              return -2.040417f;
                          }
                      } else {
                          if (features[23] < 0.281769f) {
                              return -0.112778f;
                          } else {
                              return -0.504792f;
                          }
                      }
                  } else {
                      if (features[1] < -0.101131f) {
                          if (features[0] < 28.843210f) {
                              return -0.095722f;
                          } else {
                              return -0.623529f;
                          }
                      } else {
                          if (features[0] < 34.510935f) {
                              return 0.022985f;
                          } else {
                              return -0.381154f;
                          }
                      }
                  }
              }
          } else {
              if (features[26] < -0.344918f) {
                  if (features[6] < 32.892468f) {
                      if (features[13] < -11.675500f) {
                          return 1.258000f;
                      } else {
                          if (features[20] < 0.705167f) {
                              return 0.210135f;
                          } else {
                              return 0.511380f;
                          }
                      }
                  } else {
                      if (features[22] < 6.904969f) {
                          if (features[14] < 58.867677f) {
                              return -0.055806f;
                          } else {
                              return 0.249565f;
                          }
                      } else {
                          if (features[24] < 0.101506f) {
                              return -0.356667f;
                          } else {
                              return -1.365513f;
                          }
                      }
                  }
              } else {
                  if (features[10] < -1.831744f) {
                      return 0.656000f;
                  } else {
                      if (features[26] < 0.457884f) {
                          if (features[26] < -0.064367f) {
                              return 0.230335f;
                          } else {
                              return -0.011942f;
                          }
                      } else {
                          if (features[4] < 0.119903f) {
                              return -0.059066f;
                          } else {
                              return -0.134028f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_31(const float *features, int32_t features_length) {
          if (features[25] < 0.029825f) {
              if (features[3] < -0.314634f) {
                  if (features[6] < 32.326407f) {
                      if (features[26] < -0.159819f) {
                          if (features[14] < 78.742830f) {
                              return -0.641287f;
                          } else {
                              return -0.342727f;
                          }
                      } else {
                          if (features[6] < 30.666256f) {
                              return 0.034194f;
                          } else {
                              return -0.284278f;
                          }
                      }
                  } else {
                      if (features[14] < 68.646737f) {
                          if (features[26] < -0.594857f) {
                              return -0.165909f;
                          } else {
                              return -0.841765f;
                          }
                      } else {
                          return -1.726000f;
                      }
                  }
              } else {
                  if (features[19] < -0.030537f) {
                      if (features[6] < 33.309545f) {
                          if (features[6] < 28.605742f) {
                              return -0.060297f;
                          } else {
                              return -0.147191f;
                          }
                      } else {
                          if (features[16] < 6.629205f) {
                              return -0.215833f;
                          } else {
                              return -0.640357f;
                          }
                      }
                  } else {
                      if (features[22] < 3.843418f) {
                          if (features[22] < 0.736668f) {
                              return -0.170822f;
                          } else {
                              return -0.091991f;
                          }
                      } else {
                          if (features[5] < -0.027292f) {
                              return 0.142973f;
                          } else {
                              return 0.010631f;
                          }
                      }
                  }
              }
          } else {
              if (features[24] < 0.208844f) {
                  if (features[13] < -7.213074f) {
                      if (features[0] < 34.253211f) {
                          if (features[14] < 79.542237f) {
                              return 1.133158f;
                          } else {
                              return 1.908462f;
                          }
                      } else {
                          return -0.429444f;
                      }
                  } else {
                      if (features[22] < 4.572583f) {
                          if (features[21] < 0.669485f) {
                              return -0.032637f;
                          } else {
                              return 0.161602f;
                          }
                      } else {
                          if (features[14] < 97.944441f) {
                              return 0.299310f;
                          } else {
                              return 0.986085f;
                          }
                      }
                  }
              } else {
                  return -1.480548f;
              }
          }
        }
        

static inline float forecast_temperature_tree_32(const float *features, int32_t features_length) {
          if (features[20] < 0.210612f) {
              if (features[0] < 33.059192f) {
                  if (features[16] < 4.924068f) {
                      if (features[7] < 0.200272f) {
                          if (features[25] < -0.369829f) {
                              return -0.122565f;
                          } else {
                              return -0.043209f;
                          }
                      } else {
                          if (features[23] < 0.196432f) {
                              return -0.072486f;
                          } else {
                              return -0.453056f;
                          }
                      }
                  } else {
                      if (features[25] < -0.112834f) {
                          if (features[26] < -0.684639f) {
                              return -0.062941f;
                          } else {
                              return -0.618750f;
                          }
                      } else {
                          if (features[22] < 8.608826f) {
                              return 0.232921f;
                          } else {
                              return 1.363971f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 6.747800f) {
                      if (features[2] < -0.221433f) {
                          return -0.510000f;
                      } else {
                          if (features[25] < -0.470427f) {
                              return -0.114194f;
                          } else {
                              return 0.113378f;
                          }
                      }
                  } else {
                      if (features[23] < 0.202225f) {
                          if (features[6] < 34.893654f) {
                              return -0.513830f;
                          } else {
                              return -1.346617f;
                          }
                      } else {
                          return -1.698333f;
                      }
                  }
              }
          } else {
              if (features[12] < -7.073928f) {
                  if (features[15] < 4.466290f) {
                      return 1.743333f;
                  } else {
                      return 0.848125f;
                  }
              } else {
                  if (features[25] < 0.012127f) {
                      if (features[19] < 0.043579f) {
                          if (features[6] < 29.087193f) {
                              return 0.075263f;
                          } else {
                              return -0.240805f;
                          }
                      } else {
                          if (features[5] < 0.058759f) {
                              return 0.078638f;
                          } else {
                              return -0.075556f;
                          }
                      }
                  } else {
                      if (features[25] < 0.907519f) {
                          if (features[19] < 0.125192f) {
                              return 0.184066f;
                          } else {
                              return 0.444040f;
                          }
                      } else {
                          if (features[8] < 69.504170f) {
                              return 0.022857f;
                          } else {
                              return 0.165904f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_33(const float *features, int32_t features_length) {
          if (features[25] < -0.389470f) {
              if (features[4] < -1.197549f) {
                  if (features[14] < 82.523403f) {
                      return -1.324500f;
                  } else {
                      return -0.450000f;
                  }
              } else {
                  if (features[16] < 6.717042f) {
                      if (features[6] < 29.013215f) {
                          if (features[25] < -0.711568f) {
                              return 0.016918f;
                          } else {
                              return -0.094773f;
                          }
                      } else {
                          if (features[1] < -0.175046f) {
                              return -0.719351f;
                          } else {
                              return -0.154831f;
                          }
                      }
                  } else {
                      if (features[20] < 0.038619f) {
                          if (features[11] < 1.943991f) {
                              return -0.573542f;
                          } else {
                              return -0.937500f;
                          }
                      } else {
                          return -0.445500f;
                      }
                  }
              }
          } else {
              if (features[22] < 2.385849f) {
                  if (features[0] < 25.917360f) {
                      if (features[5] < -0.198393f) {
                          return 0.024762f;
                      } else {
                          return 0.245333f;
                      }
                  } else {
                      if (features[3] < 0.109932f) {
                          if (features[20] < -0.970240f) {
                              return 0.045909f;
                          } else {
                              return -0.056418f;
                          }
                      } else {
                          if (features[10] < -0.862349f) {
                              return -0.182727f;
                          } else {
                              return -0.117647f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -7.441678f) {
                      if (features[8] < 68.878850f) {
                          if (features[13] < -16.339068f) {
                              return 0.823182f;
                          } else {
                              return -0.531765f;
                          }
                      } else {
                          if (features[7] < 0.538517f) {
                              return 2.942667f;
                          } else {
                              return 0.682500f;
                          }
                      }
                  } else {
                      if (features[22] < 9.363168f) {
                          if (features[22] < 6.161183f) {
                              return 0.122295f;
                          } else {
                              return 0.441825f;
                          }
                      } else {
                          return -1.893234f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_34(const float *features, int32_t features_length) {
          if (features[13] < -12.015705f) {
              if (features[13] < -21.120806f) {
                  return 0.588750f;
              } else {
                  return 1.493571f;
              }
          } else {
              if (features[0] < 32.469621f) {
                  if (features[19] < 0.326789f) {
                      if (features[25] < 0.283934f) {
                          if (features[6] < 27.110720f) {
                              return 0.006054f;
                          } else {
                              return -0.135122f;
                          }
                      } else {
                          if (features[26] < -0.020039f) {
                              return 0.240016f;
                          } else {
                              return -0.063849f;
                          }
                      }
                  } else {
                      if (features[26] < -0.605521f) {
                          if (features[25] < -0.135483f) {
                              return 0.054881f;
                          } else {
                              return 0.479692f;
                          }
                      } else {
                          if (features[16] < 4.233035f) {
                              return 0.064773f;
                          } else {
                              return 0.371153f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 8.834820f) {
                      if (features[14] < 72.707840f) {
                          if (features[26] < -0.837346f) {
                              return 0.106545f;
                          } else {
                              return -0.376621f;
                          }
                      } else {
                          return -1.651404f;
                      }
                  } else {
                      if (features[0] < 34.441825f) {
                          if (features[5] < -0.204203f) {
                              return 0.512941f;
                          } else {
                              return -0.322000f;
                          }
                      } else {
                          if (features[16] < 9.721285f) {
                              return -0.814737f;
                          } else {
                              return -1.940336f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_35(const float *features, int32_t features_length) {
          if (features[13] < -10.524352f) {
              if (features[25] < 0.802291f) {
                  if (features[16] < 6.702407f) {
                      return 0.456250f;
                  } else {
                      return 0.185000f;
                  }
              } else {
                  return 2.210000f;
              }
          } else {
              if (features[1] < -0.007872f) {
                  if (features[0] < 33.598100f) {
                      if (features[5] < -0.718292f) {
                          if (features[6] < 32.228484f) {
                              return -0.248611f;
                          } else {
                              return -0.835172f;
                          }
                      } else {
                          if (features[18] < -0.414236f) {
                              return -0.177623f;
                          } else {
                              return -0.023500f;
                          }
                      }
                  } else {
                      if (features[22] < 7.058453f) {
                          if (features[3] < -0.100719f) {
                              return -0.167778f;
                          } else {
                              return 0.033125f;
                          }
                      } else {
                          if (features[5] < 0.634191f) {
                              return -0.936396f;
                          } else {
                              return -1.586250f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 3.965842f) {
                      if (features[21] < 1.173293f) {
                          if (features[26] < -0.681442f) {
                              return -0.203437f;
                          } else {
                              return -0.059420f;
                          }
                      } else {
                          if (features[26] < -0.034350f) {
                              return 0.112025f;
                          } else {
                              return 0.024643f;
                          }
                      }
                  } else {
                      if (features[25] < -0.019153f) {
                          if (features[5] < -0.581595f) {
                              return 0.160568f;
                          } else {
                              return -0.104151f;
                          }
                      } else {
                          if (features[11] < -1.905353f) {
                              return 0.662434f;
                          } else {
                              return 0.266121f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_36(const float *features, int32_t features_length) {
          if (features[25] < -0.644422f) {
              if (features[16] < 5.060331f) {
                  if (features[21] < -1.489387f) {
                      if (features[7] < 0.099124f) {
                          if (features[23] < 0.308143f) {
                              return 0.010000f;
                          } else {
                              return 0.038125f;
                          }
                      } else {
                          if (features[22] < 3.854600f) {
                              return -0.497105f;
                          } else {
                              return -0.096296f;
                          }
                      }
                  } else {
                      if (features[26] < -0.009380f) {
                          if (features[25] < -0.736116f) {
                              return 0.020531f;
                          } else {
                              return -0.190135f;
                          }
                      } else {
                          if (features[22] < 2.616111f) {
                              return -0.188802f;
                          } else {
                              return -0.065918f;
                          }
                      }
                  }
              } else {
                  if (features[2] < -0.315816f) {
                      return -1.236914f;
                  } else {
                      if (features[25] < -0.944056f) {
                          return -0.523478f;
                      } else {
                          if (features[26] < -0.525413f) {
                              return -0.860714f;
                          } else {
                              return -0.518824f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 5.378115f) {
                  if (features[25] < 0.591749f) {
                      if (features[10] < 2.212760f) {
                          if (features[25] < -0.219835f) {
                              return -0.153668f;
                          } else {
                              return -0.033883f;
                          }
                      } else {
                          if (features[23] < 0.271739f) {
                              return -0.462778f;
                          } else {
                              return -0.661778f;
                          }
                      }
                  } else {
                      if (features[22] < 2.315695f) {
                          if (features[19] < 0.164739f) {
                              return -0.071031f;
                          } else {
                              return 0.045111f;
                          }
                      } else {
                          if (features[16] < 4.153163f) {
                              return 0.112361f;
                          } else {
                              return 0.282131f;
                          }
                      }
                  }
              } else {
                  if (features[26] < -0.380869f) {
                      if (features[24] < 0.236804f) {
                          if (features[6] < 33.159406f) {
                              return 0.434864f;
                          } else {
                              return -0.097737f;
                          }
                      } else {
                          return -1.480548f;
                      }
                  } else {
                      return 1.369524f;
                  }
              }
          }
        }
        

static inline float forecast_temperature_tree_37(const float *features, int32_t features_length) {
          if (features[0] < 33.301411f) {
              if (features[13] < -7.203221f) {
                  if (features[5] < 0.931415f) {
                      return 2.030000f;
                  } else {
                      if (features[7] < 0.711720f) {
                          return 1.486667f;
                      } else {
                          return 1.012000f;
                      }
                  }
              } else {
                  if (features[22] < 4.166202f) {
                      if (features[15] < 2.091534f) {
                          if (features[21] < 2.046553f) {
                              return -0.053870f;
                          } else {
                              return 0.135458f;
                          }
                      } else {
                          if (features[22] < 2.503853f) {
                              return -0.217273f;
                          } else {
                              return -0.491053f;
                          }
                      }
                  } else {
                      if (features[25] < -0.218587f) {
                          if (features[19] < -0.028255f) {
                              return -0.332886f;
                          } else {
                              return 0.062532f;
                          }
                      } else {
                          if (features[25] < -0.038380f) {
                              return -0.170610f;
                          } else {
                              return 0.312489f;
                          }
                      }
                  }
              }
          } else {
              if (features[23] < 0.401089f) {
                  if (features[16] < 7.669468f) {
                      if (features[19] < -0.120043f) {
                          if (features[3] < -0.065281f) {
                              return -0.781765f;
                          } else {
                              return -0.409412f;
                          }
                      } else {
                          if (features[0] < 35.381189f) {
                              return 0.186418f;
                          } else {
                              return -0.201000f;
                          }
                      }
                  } else {
                      if (features[22] < 10.715404f) {
                          if (features[19] < -0.069318f) {
                              return -0.839211f;
                          } else {
                              return -0.270357f;
                          }
                      } else {
                          return -1.085048f;
                      }
                  }
              } else {
                  return -1.516444f;
              }
          }
        }
        

static inline float forecast_temperature_tree_38(const float *features, int32_t features_length) {
          if (features[25] < -0.674138f) {
              if (features[6] < 33.869818f) {
                  if (features[22] < 4.821867f) {
                      if (features[21] < -2.063181f) {
                          if (features[17] < -0.209134f) {
                              return -0.416667f;
                          } else {
                              return -0.220000f;
                          }
                      } else {
                          if (features[15] < 1.701008f) {
                              return -0.049818f;
                          } else {
                              return -0.201873f;
                          }
                      }
                  } else {
                      if (features[8] < 75.164171f) {
                          return -0.703158f;
                      } else {
                          return -0.470444f;
                      }
                  }
              } else {
                  if (features[23] < 0.279361f) {
                      if (features[12] < 2.603683f) {
                          return -0.965000f;
                      } else {
                          return -0.218125f;
                      }
                  } else {
                      return -1.637451f;
                  }
              }
          } else {
              if (features[24] < 0.045188f) {
                  if (features[13] < -12.257177f) {
                      if (features[25] < 0.746187f) {
                          return 0.381818f;
                      } else {
                          return 2.035882f;
                      }
                  } else {
                      if (features[25] < 0.242418f) {
                          if (features[25] < -0.044053f) {
                              return -0.138114f;
                          } else {
                              return 0.024715f;
                          }
                      } else {
                          if (features[26] < 0.025947f) {
                              return 0.265093f;
                          } else {
                              return -0.058332f;
                          }
                      }
                  }
              } else {
                  return -1.365513f;
              }
          }
        }
        

static inline float forecast_temperature_tree_39(const float *features, int32_t features_length) {
          if (features[25] < -0.534282f) {
              if (features[16] < 6.177098f) {
                  if (features[2] < -0.285984f) {
                      if (features[4] < -0.590790f) {
                          if (features[25] < -0.872799f) {
                              return -0.334912f;
                          } else {
                              return -0.687895f;
                          }
                      } else {
                          return -0.022273f;
                      }
                  } else {
                      if (features[16] < 0.819256f) {
                          if (features[8] < 77.626681f) {
                              return -0.459412f;
                          } else {
                              return -0.211370f;
                          }
                      } else {
                          if (features[25] < -0.919712f) {
                              return -0.016950f;
                          } else {
                              return -0.123757f;
                          }
                      }
                  }
              } else {
                  if (features[21] < -1.023477f) {
                      return -1.564561f;
                  } else {
                      if (features[5] < 0.331878f) {
                          if (features[20] < -0.371651f) {
                              return -0.437500f;
                          } else {
                              return -0.725217f;
                          }
                      } else {
                          return -0.317143f;
                      }
                  }
              }
          } else {
              if (features[16] < 4.752689f) {
                  if (features[25] < -0.065392f) {
                      if (features[26] < 0.156193f) {
                          if (features[2] < -0.040831f) {
                              return -0.439630f;
                          } else {
                              return -0.102978f;
                          }
                      } else {
                          if (features[21] < -1.032367f) {
                              return 0.035333f;
                          } else {
                              return -0.085098f;
                          }
                      }
                  } else {
                      if (features[21] < 0.328784f) {
                          if (features[26] < -0.173747f) {
                              return 0.009242f;
                          } else {
                              return -0.058006f;
                          }
                      } else {
                          if (features[23] < 0.520536f) {
                              return 0.161945f;
                          } else {
                              return 0.049074f;
                          }
                      }
                  }
              } else {
                  if (features[14] < 81.946042f) {
                      if (features[8] < 78.735835f) {
                          if (features[24] < 0.199379f) {
                              return 0.196335f;
                          } else {
                              return -1.480548f;
                          }
                      } else {
                          return 0.763827f;
                      }
                  } else {
                      if (features[13] < -5.595971f) {
                          if (features[15] < 1.639452f) {
                              return 0.908667f;
                          } else {
                              return 2.316667f;
                          }
                      } else {
                          if (features[8] < 97.287473f) {
                              return 0.348500f;
                          } else {
                              return 1.090069f;
                          }
                      }
                  }
              }
          }
        }
        

static const uint8_t forecast_temperature_leaves[3352] = { 45, 33, 159, 190, 211, 74, 79, 191, 159, 59, 224, 189, 9, 248, 158, 62, 181, 235, 191, 61, 159, 151, 153, 62, 226, 196, 129, 189, 204, 5, 170, 187, 217, 43, 23, 191, 222, 221, 225, 63, 158, 148, 51, 63, 220, 28, 59, 190, 76, 45, 178, 191, 115, 41, 23, 191, 209, 72, 0, 192, 46, 3, 27, 191, 209, 223, 73, 190, 61, 81, 255, 188, 100, 63, 246, 62, 94, 196, 106, 191, 99, 38, 238, 190, 254, 156, 29, 190, 195, 246, 106, 189, 211, 142, 28, 190, 250, 59, 217, 190, 190, 121, 181, 191, 89, 181, 13, 62, 33, 122, 23, 191, 100, 33, 139, 190, 225, 122, 28, 64, 151, 219, 175, 63, 200, 185, 237, 191, 158, 228, 78, 62, 198, 52, 89, 63, 147, 208, 7, 62, 176, 78, 170, 61, 98, 165, 43, 189, 42, 62, 53, 190, 168, 14, 147, 189, 62, 121, 152, 191, 41, 92, 207, 190, 168, 236, 112, 189, 2, 190, 137, 62, 121, 74, 16, 62, 239, 238, 50, 191, 16, 44, 2, 191, 158, 236, 112, 190, 255, 138, 236, 189, 159, 125, 176, 189, 113, 151, 25, 60, 141, 176, 92, 190, 150, 19, 52, 189, 230, 117, 206, 189, 123, 193, 27, 62, 115, 210, 124, 61, 37, 201, 174, 191, 48, 150, 2, 64, 174, 71, 113, 190, 191, 182, 56, 63, 120, 175, 85, 62, 217, 216, 216, 60, 35, 188, 213, 62, 234, 35, 219, 188, 207, 172, 79, 190, 218, 3, 219, 190, 189, 18, 106, 190, 226, 46, 76, 190, 183, 26, 174, 189, 247, 144, 186, 191, 38, 254, 226, 189, 151, 219, 223, 190, 45, 230, 37, 191, 206, 177, 219, 189, 113, 135, 17, 191, 205, 204, 132, 62, 197, 221, 222, 62, 8, 172, 36, 64, 233, 249, 77, 63, 164, 76, 109, 189, 22, 124, 98, 190, 156, 53, 207, 61, 0, 0, 0, 189, 64, 10, 128, 61, 139, 91, 136, 62, 27, 47, 253, 190, 97, 90, 139, 190, 246, 131, 125, 61, 164, 144, 225, 189, 187, 196, 234, 189, 112, 72, 159, 190, 172, 157, 92, 189, 42, 195, 156, 61, 205, 204, 76, 189, 149, 123, 18, 62, 83, 131, 85, 190, 42, 71, 32, 191, 112, 104, 173, 62, 15, 152, 239, 186, 184, 121, 11, 62, 67, 109, 231, 62, 16, 39, 3, 64, 15, 30, 243, 62, 48, 27, 135, 191, 225, 122, 44, 190, 251, 61, 183, 188, 33, 33, 161, 62, 69, 127, 155, 191, 208, 105, 131, 191, 55, 199, 14, 191, 80, 141, 87, 191, 122, 53, 113, 189, 141, 67, 103, 189, 76, 104, 239, 190, 170, 241, 218, 190, 46, 70, 138, 191, 22, 141, 164, 191, 116, 180, 134, 189, 175, 104, 162, 61, 30, 3, 199, 190, 101, 68, 246, 189, 13, 125, 85, 61, 8, 5, 148, 189, 63, 233, 19, 62, 186, 212, 36, 61, 170, 234, 28, 191, 173, 163, 103, 62, 46, 80, 2, 63, 118, 108, 65, 63, 79, 241, 254, 63, 240, 96, 158, 63, 221, 103, 247, 63, 161, 14, 138, 63, 51, 122, 219, 62, 59, 168, 83, 191, 144, 205, 116, 190, 148, 42, 95, 191, 69, 163, 14, 190, 96, 8, 112, 188, 151, 121, 182, 189, 198, 53, 10, 189, 211, 178, 133, 61, 224, 202, 195, 62, 80, 102, 26, 62, 99, 70, 255, 62, 240, 182, 167, 61, 35, 77, 98, 189, 184, 30, 157, 63, 58, 109, 240, 62, 10, 215, 15, 191, 32, 103, 150, 189, 53, 36, 149, 60, 19, 228, 41, 190, 219, 185, 5, 191, 82, 184, 114, 191, 252, 169, 177, 190, 25, 96, 78, 189, 92, 103, 184, 61, 235, 226, 182, 190, 92, 143, 66, 62, 125, 190, 33, 188, 147, 17, 252, 189, 44, 173, 209, 189, 135, 59, 133, 188, 255, 78, 214, 61, 7, 162, 94, 189, 143, 194, 53, 190, 162, 248, 184, 191, 126, 118, 129, 190, 49, 218, 47, 191, 112, 136, 34, 190, 114, 151, 37, 191, 189, 235, 104, 189, 98, 134, 79, 62, 116, 167, 218, 62, 74, 12, 34, 64, 149, 6, 191, 191, 179, 221, 168, 62, 130, 15, 161, 191, 19, 218, 11, 191, 133, 117, 130, 189, 144, 101, 83, 61, 210, 252, 100, 62, 229, 58, 15, 191, 165, 56, 234, 189, 36, 224, 203, 62, 35, 240, 140, 191, 199, 156, 148, 63, 161, 3, 105, 63, 183, 189, 66, 62, 163, 250, 54, 63, 147, 11, 154, 190, 151, 55, 34, 191, 108, 204, 87, 191, 182, 243, 253, 190, 125, 22, 18, 189, 125, 241, 25, 190, 67, 175, 51, 61, 133, 105, 120, 62, 200, 187, 90, 189, 237, 82, 57, 190, 168, 175, 173, 62, 155, 120, 14, 64, 189, 49, 130, 63, 242, 148, 224, 190, 52, 21, 144, 61, 201, 98, 124, 190, 173, 142, 137, 62, 218, 64, 209, 191, 195, 245, 32, 191, 225, 185, 238, 191, 53, 94, 58, 191, 147, 164, 132, 61, 205, 204, 148, 190, 247, 91, 145, 62, 123, 20, 240, 63, 148, 16, 6, 64, 161, 214, 52, 63, 223, 83, 36, 62, 214, 225, 81, 191, 205, 104, 75, 61, 176, 69, 209, 189, 57, 109, 26, 190, 221, 131, 2, 191, 209, 181, 141, 191, 10, 154, 32, 191, 41, 92, 15, 191, 80, 192, 126, 61, 68, 122, 105, 189, 137, 136, 8, 62, 122, 217, 74, 189, 120, 1, 18, 62, 96, 117, 21, 190, 96, 40, 226, 61, 160, 249, 148, 62, 159, 56, 82, 191, 163, 77, 55, 60, 220, 183, 28, 189, 221, 200, 172, 189, 247, 194, 192, 60, 61, 10, 55, 191, 91, 39, 244, 187, 239, 241, 87, 190, 119, 115, 213, 63, 209, 120, 188, 62, 164, 112, 189, 63, 180, 92, 205, 190, 230, 146, 117, 189, 234, 62, 41, 62, 14, 67, 180, 190, 149, 205, 148, 191, 247, 246, 246, 189, 58, 109, 32, 191, 89, 242, 251, 190, 245, 222, 165, 191, 75, 117, 246, 62, 83, 31, 108, 60, 20, 245, 239, 190, 150, 130, 189, 191, 54, 95, 24, 191, 174, 81, 243, 189, 79, 86, 11, 191, 93, 25, 178, 61, 187, 196, 86, 63, 104, 145, 177, 62, 250, 66, 102, 60, 193, 179, 0, 190, 182, 106, 78, 62, 113, 61, 4, 64, 30, 95, 75, 189, 40, 37, 195, 61, 157, 76, 29, 62, 123, 20, 174, 60, 137, 136, 72, 63, 187, 41, 215, 62, 125, 230, 244, 61, 106, 163, 231, 189, 226, 74, 246, 59, 133, 235, 113, 191, 3, 3, 195, 190, 230, 19, 250, 60, 22, 178, 16, 191, 225, 122, 8, 63, 134, 168, 74, 63, 116, 157, 66, 190, 145, 126, 59, 191, 119, 81, 146, 190, 222, 61, 102, 189, 154, 153, 149, 191, 86, 76, 154, 190, 20, 174, 199, 62, 11, 205, 133, 189, 79, 27, 232, 61, 32, 21, 234, 187, 96, 243, 16, 62, 224, 201, 234, 62, 143, 194, 27, 64, 46, 29, 4, 62, 23, 109, 239, 62, 88, 118, 178, 188, 23, 47, 170, 190, 244, 47, 16, 189, 160, 223, 243, 189, 206, 234, 96, 189, 252, 30, 139, 62, 104, 216, 5, 64, 60, 117, 120, 62, 172, 65, 165, 191, 62, 166, 213, 61, 51, 204, 181, 190, 215, 163, 48, 190, 186, 73, 44, 191, 47, 32, 6, 192, 59, 223, 79, 63, 21, 249, 140, 62, 109, 160, 211, 62, 166, 32, 52, 62, 233, 97, 134, 61, 95, 41, 75, 189, 22, 124, 226, 61, 38, 20, 5, 64, 72, 225, 6, 63, 37, 200, 211, 60, 69, 35, 1, 191, 126, 79, 88, 191, 93, 110, 223, 189, 146, 215, 186, 190, 135, 59, 166, 189, 13, 218, 166, 62, 33, 234, 164, 191, 171, 170, 42, 62, 66, 25, 189, 190, 159, 244, 201, 190, 121, 233, 38, 191, 106, 33, 102, 62, 41, 27, 253, 63, 214, 116, 125, 189, 211, 77, 226, 61, 53, 212, 158, 189, 69, 221, 77, 62, 76, 220, 165, 190, 193, 239, 140, 191, 170, 188, 32, 191, 55, 199, 110, 190, 55, 177, 25, 189, 172, 134, 30, 191, 29, 142, 150, 190, 38, 14, 138, 188, 0, 0, 0, 191, 91, 98, 197, 189, 214, 168, 156, 190, 41, 92, 143, 61, 215, 163, 48, 62, 156, 87, 147, 62, 123, 20, 174, 61, 237, 225, 73, 189, 249, 244, 53, 190, 22, 154, 212, 189, 116, 218, 192, 61, 82, 184, 14, 64, 231, 29, 125, 63, 205, 159, 120, 62, 61, 215, 69, 62, 11, 89, 72, 190, 225, 222, 53, 191, 192, 148, 68, 191, 76, 109, 51, 190, 2, 238, 133, 189, 209, 5, 66, 190, 166, 155, 4, 62, 154, 153, 121, 63, 195, 245, 40, 62, 51, 51, 71, 191, 197, 87, 156, 191, 176, 31, 12, 191, 30, 130, 125, 191, 35, 203, 75, 190, 251, 153, 14, 61, 10, 215, 35, 188, 0, 0, 32, 62, 214, 24, 34, 189, 162, 191, 147, 61, 172, 156, 234, 188, 203, 148, 185, 189, 241, 38, 71, 62, 223, 161, 203, 62, 53, 211, 8, 64, 202, 201, 201, 61, 117, 230, 192, 190, 101, 12, 11, 63, 205, 204, 254, 63, 142, 80, 198, 63, 75, 126, 49, 61, 183, 233, 92, 63, 248, 230, 21, 191, 7, 51, 130, 189, 37, 4, 129, 190, 144, 130, 1, 191, 253, 245, 187, 191, 107, 9, 181, 190, 240, 5, 184, 188, 1, 223, 188, 61, 134, 126, 134, 189, 162, 178, 195, 188, 31, 133, 251, 61, 73, 146, 84, 190, 18, 191, 92, 62, 235, 71, 245, 189, 44, 249, 197, 61, 61, 10, 26, 64, 231, 162, 110, 191, 163, 45, 165, 189, 210, 39, 253, 190, 171, 239, 238, 60, 29, 212, 193, 62, 30, 216, 253, 190, 92, 182, 89, 189, 245, 111, 68, 62, 61, 10, 19, 191, 239, 29, 146, 190, 186, 71, 113, 62, 206, 125, 26, 191, 133, 235, 17, 62, 56, 61, 4, 190, 208, 123, 249, 188, 121, 94, 178, 190, 0, 228, 57, 191, 245, 67, 13, 191, 120, 195, 225, 191, 160, 191, 193, 189, 109, 160, 83, 62, 205, 204, 204, 60, 102, 102, 86, 191, 113, 61, 74, 62, 136, 145, 131, 61, 21, 209, 140, 189, 71, 52, 13, 61, 76, 55, 9, 62, 239, 238, 238, 61, 191, 48, 143, 189, 161, 67, 175, 61, 123, 112, 235, 188, 73, 244, 39, 62, 10, 215, 35, 188, 106, 10, 176, 62, 206, 120, 131, 63, 176, 5, 227, 63, 255, 71, 42, 63, 89, 242, 11, 62, 35, 229, 177, 191, 106, 3, 177, 190, 116, 200, 228, 189, 69, 112, 251, 190, 186, 166, 187, 191, 117, 168, 157, 189, 206, 117, 118, 186, 183, 240, 120, 61, 71, 144, 17, 63, 85, 6, 128, 190, 225, 44, 21, 189, 96, 229, 208, 61, 14, 116, 218, 185, 245, 111, 4, 61, 67, 116, 212, 189, 238, 175, 106, 62, 222, 118, 69, 63, 222, 221, 29, 190, 137, 136, 168, 63, 68, 236, 85, 63, 238, 133, 206, 62, 48, 217, 58, 191, 190, 159, 154, 190, 253, 193, 191, 189, 18, 200, 81, 190, 252, 107, 104, 189, 248, 42, 81, 59, 4, 237, 109, 62, 242, 139, 165, 63, 93, 202, 208, 191, 151, 55, 6, 191, 16, 179, 13, 189, 133, 235, 49, 191, 96, 44, 181, 191, 178, 135, 92, 61, 185, 8, 131, 62, 240, 128, 211, 62, 118, 20, 104, 62, 109, 44, 28, 61, 135, 46, 173, 189, 194, 40, 15, 190, 123, 20, 110, 190, 139, 108, 55, 64, 96, 96, 160, 63, 113, 61, 170, 61, 115, 64, 24, 62, 132, 159, 135, 190, 196, 107, 135, 62, 157, 54, 40, 63, 213, 35, 3, 62, 223, 11, 148, 189, 58, 170, 6, 190, 131, 216, 117, 191, 221, 162, 122, 189, 235, 237, 4, 191, 63, 192, 150, 191, 10, 215, 30, 64, 155, 120, 86, 63, 156, 196, 128, 190, 58, 46, 102, 62, 159, 114, 115, 63, 52, 36, 4, 189, 57, 250, 17, 62, 236, 81, 200, 61, 2, 208, 109, 189, 145, 90, 138, 188, 59, 178, 44, 62, 135, 162, 27, 191, 115, 242, 23, 190, 212, 110, 0, 61, 123, 20, 46, 191, 223, 84, 6, 191, 187, 247, 162, 190, 150, 137, 164, 189, 72, 225, 90, 62, 104, 164, 105, 189, 69, 170, 119, 60, 100, 13, 163, 61, 245, 111, 4, 189, 202, 29, 32, 62, 236, 81, 184, 61, 150, 222, 201, 63, 193, 202, 97, 190, 15, 255, 69, 61, 191, 206, 153, 62, 34, 203, 41, 190, 143, 156, 208, 190, 217, 143, 125, 58, 247, 143, 189, 189, 0, 0, 128, 190, 230, 249, 16, 191, 166, 239, 203, 190, 112, 41, 169, 189, 38, 191, 168, 62, 194, 35, 188, 191, 175, 186, 87, 191, 57, 210, 235, 190, 112, 9, 104, 191, 204, 114, 113, 190, 178, 241, 103, 62, 110, 176, 177, 63, 211, 148, 21, 61, 121, 184, 143, 189, 126, 12, 231, 61, 160, 60, 140, 62, 58, 100, 165, 61, 49, 114, 112, 191, 63, 222, 18, 191, 248, 242, 185, 190, 85, 55, 140, 190, 57, 237, 180, 189, 7, 115, 85, 62, 88, 19, 207, 188, 52, 224, 128, 191, 51, 62, 52, 191, 129, 48, 82, 189, 16, 155, 218, 63, 23, 108, 65, 62, 183, 92, 54, 191, 11, 243, 93, 63, 142, 212, 128, 62, 10, 215, 163, 61, 240, 145, 71, 189, 0, 0, 0, 190, 51, 51, 179, 188, 93, 116, 209, 61, 76, 185, 148, 62, 58, 146, 235, 63, 121, 65, 149, 191, 127, 237, 54, 191, 208, 105, 131, 190, 247, 47, 135, 190, 114, 197, 141, 189, 109, 169, 238, 190, 112, 88, 123, 191, 60, 240, 182, 191, 103, 115, 152, 189, 54, 42, 136, 60, 205, 159, 8, 62, 10, 164, 64, 63, 249, 138, 175, 190, 14, 22, 148, 190, 7, 44, 130, 187, 75, 53, 44, 191, 139, 184, 162, 189, 61, 254, 53, 190, 47, 91, 161, 191, 114, 73, 240, 190, 186, 73, 140, 62, 79, 195, 242, 190, 249, 146, 44, 191, 11, 122, 184, 191, 140, 160, 215, 59, 48, 203, 29, 62, 229, 27, 153, 60, 248, 189, 150, 189, 80, 175, 7, 61, 87, 48, 117, 62, 91, 104, 49, 63, 135, 181, 191, 63, 92, 54, 130, 61, 118, 198, 205, 61, 102, 102, 38, 190, 21, 162, 35, 190, 110, 220, 81, 191, 234, 182, 3, 60, 74, 12, 66, 191, 143, 100, 162, 189, 151, 26, 172, 61, 244, 243, 243, 190, 127, 253, 110, 190, 116, 51, 65, 190, 231, 184, 68, 189, 87, 226, 3, 63, 247, 130, 7, 64, 122, 214, 98, 191, 193, 204, 69, 189, 215, 86, 253, 61, 242, 28, 144, 62, 126, 248, 252, 62, 247, 52, 175, 188, 58, 211, 108, 190, 47, 15, 1, 191, 96, 229, 208, 190, 39, 158, 71, 190, 119, 227, 148, 189, 225, 122, 44, 191, 48, 150, 2, 192, 9, 248, 230, 189, 7, 58, 1, 191, 3, 10, 196, 189, 160, 159, 31, 191, 50, 75, 188, 60, 153, 38, 195, 190, 37, 6, 161, 63, 145, 45, 87, 62, 208, 233, 2, 63, 78, 149, 100, 189, 6, 142, 127, 62, 3, 157, 182, 190, 158, 239, 39, 63, 247, 220, 107, 62, 94, 166, 67, 188, 73, 239, 113, 189, 148, 62, 9, 190, 91, 43, 36, 191, 243, 121, 175, 190, 137, 14, 12, 61, 219, 140, 145, 190, 145, 237, 220, 191, 228, 125, 87, 191, 5, 250, 118, 189, 67, 185, 22, 190, 106, 3, 93, 190, 114, 238, 35, 191, 226, 235, 46, 190, 194, 101, 188, 189, 148, 103, 18, 62, 25, 44, 46, 60, 36, 224, 219, 190, 81, 11, 145, 63, 120, 72, 244, 63, 144, 174, 5, 189, 249, 122, 37, 62, 56, 63, 153, 62, 11, 112, 124, 63, 67, 3, 251, 189, 108, 252, 48, 189, 153, 115, 148, 189, 230, 246, 231, 190, 78, 231, 128, 189, 102, 102, 30, 191, 234, 130, 110, 62, 151, 150, 174, 63, 92, 143, 2, 191, 79, 222, 233, 189, 236, 50, 232, 61, 253, 98, 217, 191, 89, 138, 3, 191, 238, 93, 172, 191, 140, 37, 223, 63, 184, 30, 89, 63, 146, 35, 154, 61, 149, 149, 118, 190, 203, 12, 161, 61, 223, 188, 154, 189, 234, 123, 60, 62, 62, 89, 227, 62, 231, 62, 187, 60, 186, 226, 41, 62, 55, 137, 169, 191, 102, 102, 230, 190, 152, 152, 138, 60, 73, 24, 194, 189, 93, 39, 56, 191, 0, 140, 30, 190, 147, 24, 228, 190, 160, 211, 18, 191, 0, 0, 112, 191, 122, 217, 202, 60, 169, 56, 123, 62, 44, 11, 60, 61, 33, 22, 103, 189, 220, 28, 59, 190, 241, 240, 240, 189, 11, 188, 82, 63, 187, 33, 8, 191, 167, 84, 60, 64, 82, 184, 46, 63, 127, 85, 242, 191, 157, 117, 250, 61, 219, 54, 226, 62, 82, 184, 22, 63, 89, 45, 191, 63, 193, 94, 198, 59, 113, 93, 10, 190, 196, 198, 117, 62, 88, 195, 130, 189, 217, 202, 96, 61, 64, 154, 245, 62, 201, 167, 132, 61, 209, 7, 190, 62, 49, 97, 211, 191, 129, 52, 218, 61, 126, 212, 192, 190, 29, 80, 3, 63, 47, 221, 164, 190, 152, 146, 80, 191, 239, 92, 248, 191, 164, 112, 13, 64, 154, 153, 233, 62, 164, 112, 61, 62, 233, 147, 126, 190, 206, 205, 85, 191, 218, 226, 53, 190, 203, 130, 192, 188, 240, 205, 43, 190, 20, 174, 7, 61, 173, 183, 111, 191, 61, 10, 203, 191, 236, 81, 80, 190, 145, 98, 115, 189, 135, 109, 229, 61, 209, 223, 201, 60, 16, 108, 36, 62, 14, 77, 213, 189, 68, 149, 41, 63, 18, 65, 136, 62, 10, 215, 35, 60, 246, 40, 28, 61, 149, 132, 254, 190, 254, 54, 197, 189, 26, 48, 168, 60, 201, 178, 66, 190, 100, 85, 65, 190, 27, 0, 135, 189, 47, 83, 158, 191, 172, 2, 6, 191, 197, 87, 92, 191, 158, 209, 4, 191, 10, 91, 29, 190, 248, 200, 10, 189, 53, 241, 236, 190, 69, 106, 41, 191, 194, 120, 145, 189, 110, 198, 56, 61, 149, 29, 230, 61, 120, 115, 144, 62, 142, 76, 175, 63, 124, 166, 222, 62, 116, 42, 200, 189, 133, 235, 1, 64, 24, 75, 190, 63, 55, 137, 129, 63, 171, 166, 92, 189, 114, 181, 10, 62, 190, 124, 94, 190, 64, 107, 251, 190, 13, 112, 170, 190, 151, 16, 128, 61, 83, 180, 46, 190, 128, 254, 159, 62, 218, 26, 194, 191, 187, 33, 72, 191, 107, 158, 209, 190, 86, 228, 62, 62, 242, 210, 77, 190, 215, 226, 138, 191, 128, 214, 86, 191, 64, 108, 138, 190, 85, 85, 213, 190, 174, 71, 97, 190, 62, 14, 76, 189, 222, 183, 78, 190, 40, 2, 52, 191, 24, 222, 240, 190, 254, 151, 209, 191, 61, 10, 119, 191, 41, 92, 95, 190, 172, 125, 195, 62, 229, 75, 2, 64, 216, 109, 13, 190, 32, 118, 202, 60, 81, 186, 135, 62, 204, 237, 110, 189, 75, 117, 182, 188, 159, 121, 171, 190, 223, 25, 48, 191, 5, 56, 235, 190, 87, 113, 88, 190, 176, 218, 138, 188, 53, 116, 253, 189, 140, 67, 200, 191, 140, 96, 162, 190, 0, 0, 224, 190, 217, 167, 57, 191, 35, 23, 225, 190, 3, 230, 210, 189, 175, 185, 16, 61, 155, 71, 174, 189, 34, 108, 23, 60, 143, 151, 109, 189, 248, 212, 37, 62, 229, 1, 73, 61, 45, 138, 67, 63, 7, 12, 73, 62, 97, 158, 104, 63, 68, 68, 20, 64, 152, 110, 178, 62, 101, 135, 139, 63 };

float forecast_temperature_predict(const float *features, int32_t features_length) {

        float avg = 0;

        avg += forecast_temperature_tree_0(features, features_length); 
    avg += forecast_temperature_tree_1(features, features_length); 
    avg += forecast_temperature_tree_2(features, features_length); 
    avg += forecast_temperature_tree_3(features, features_length); 
    avg += forecast_temperature_tree_4(features, features_length); 
    avg += forecast_temperature_tree_5(features, features_length); 
    avg += forecast_temperature_tree_6(features, features_length); 
    avg += forecast_temperature_tree_7(features, features_length); 
    avg += forecast_temperature_tree_8(features, features_length); 
    avg += forecast_temperature_tree_9(features, features_length); 
    avg += forecast_temperature_tree_10(features, features_length); 
    avg += forecast_temperature_tree_11(features, features_length); 
    avg += forecast_temperature_tree_12(features, features_length); 
    avg += forecast_temperature_tree_13(features, features_length); 
    avg += forecast_temperature_tree_14(features, features_length); 
    avg += forecast_temperature_tree_15(features, features_length); 
    avg += forecast_temperature_tree_16(features, features_length); 
    avg += forecast_temperature_tree_17(features, features_length); 
    avg += forecast_temperature_tree_18(features, features_length); 
    avg += forecast_temperature_tree_19(features, features_length); 
    avg += forecast_temperature_tree_20(features, features_length); 
    avg += forecast_temperature_tree_21(features, features_length); 
    avg += forecast_temperature_tree_22(features, features_length); 
    avg += forecast_temperature_tree_23(features, features_length); 
    avg += forecast_temperature_tree_24(features, features_length); 
    avg += forecast_temperature_tree_25(features, features_length); 
    avg += forecast_temperature_tree_26(features, features_length); 
    avg += forecast_temperature_tree_27(features, features_length); 
    avg += forecast_temperature_tree_28(features, features_length); 
    avg += forecast_temperature_tree_29(features, features_length); 
    avg += forecast_temperature_tree_30(features, features_length); 
    avg += forecast_temperature_tree_31(features, features_length); 
    avg += forecast_temperature_tree_32(features, features_length); 
    avg += forecast_temperature_tree_33(features, features_length); 
    avg += forecast_temperature_tree_34(features, features_length); 
    avg += forecast_temperature_tree_35(features, features_length); 
    avg += forecast_temperature_tree_36(features, features_length); 
    avg += forecast_temperature_tree_37(features, features_length); 
    avg += forecast_temperature_tree_38(features, features_length); 
    avg += forecast_temperature_tree_39(features, features_length); 
        
        return avg/40;
    }
    
#endif // FORECAST_TEMPERATURE_H
