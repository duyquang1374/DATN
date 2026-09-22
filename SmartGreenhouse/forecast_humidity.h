#ifndef FORECAST_HUMIDITY_H
#define FORECAST_HUMIDITY_H




    // !!! This file is generated using emlearn !!!

    #include <stdint.h>
    

static inline float forecast_humidity_tree_0(const float *features, int32_t features_length) {
          if (features[25] < -0.506937f) {
              if (features[22] < 5.473162f) {
                  if (features[8] < 93.508325f) {
                      if (features[21] < -1.767335f) {
                          if (features[8] < 78.565186f) {
                              return 0.312500f;
                          } else {
                              return 2.578696f;
                          }
                      } else {
                          if (features[22] < 1.807544f) {
                              return 1.430308f;
                          } else {
                              return 0.507181f;
                          }
                      }
                  } else {
                      if (features[25] < -0.867735f) {
                          if (features[25] < -0.959081f) {
                              return -0.107021f;
                          } else {
                              return 1.029000f;
                          }
                      } else {
                          if (features[13] < -0.147787f) {
                              return 0.330556f;
                          } else {
                              return -0.423551f;
                          }
                      }
                  }
              } else {
                  if (features[21] < -1.444060f) {
                      return 4.563426f;
                  } else {
                      if (features[3] < -0.165738f) {
                          if (features[23] < 0.226128f) {
                              return 2.686170f;
                          } else {
                              return 3.873210f;
                          }
                      } else {
                          if (features[11] < -0.081272f) {
                              return 1.206471f;
                          } else {
                              return 0.302778f;
                          }
                      }
                  }
              }
          } else {
              if (features[13] < -4.243835f) {
                  if (features[26] < -0.874431f) {
                      if (features[22] < 5.931408f) {
                          return 0.591746f;
                      } else {
                          return -0.692667f;
                      }
                  } else {
                      if (features[8] < 76.437251f) {
                          if (features[13] < -15.773940f) {
                              return -4.353200f;
                          } else {
                              return -0.137530f;
                          }
                      } else {
                          if (features[8] < 85.421501f) {
                              return -5.997442f;
                          } else {
                              return -4.417647f;
                          }
                      }
                  }
              } else {
                  if (features[25] < 0.157052f) {
                      if (features[16] < 5.934672f) {
                          if (features[26] < 0.651423f) {
                              return 1.258710f;
                          } else {
                              return 0.299780f;
                          }
                      } else {
                          if (features[12] < -0.340155f) {
                              return -0.115571f;
                          } else {
                              return -1.365283f;
                          }
                      }
                  } else {
                      if (features[23] < 0.152364f) {
                          if (features[21] < 0.359131f) {
                              return 0.076427f;
                          } else {
                              return -1.334511f;
                          }
                      } else {
                          if (features[22] < 5.377306f) {
                              return -0.528677f;
                          } else {
                              return -1.231693f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_1(const float *features, int32_t features_length) {
          if (features[25] < -0.104729f) {
              if (features[5] < -0.627229f) {
                  if (features[0] < 27.403130f) {
                      if (features[6] < 27.156761f) {
                          if (features[21] < 0.066756f) {
                              return -0.675000f;
                          } else {
                              return -1.167778f;
                          }
                      } else {
                          return 0.511250f;
                      }
                  } else {
                      if (features[8] < 87.638217f) {
                          if (features[0] < 28.909092f) {
                              return 0.234545f;
                          } else {
                              return 2.894170f;
                          }
                      } else {
                          if (features[8] < 91.321474f) {
                              return 0.845600f;
                          } else {
                              return -1.015000f;
                          }
                      }
                  }
              } else {
                  if (features[26] < -0.622782f) {
                      if (features[9] < 1.569338f) {
                          if (features[6] < 26.922451f) {
                              return -0.824118f;
                          } else {
                              return 0.996344f;
                          }
                      } else {
                          return 3.165000f;
                      }
                  } else {
                      if (features[14] < 71.118094f) {
                          if (features[16] < 2.930773f) {
                              return 1.448058f;
                          } else {
                              return 0.578106f;
                          }
                      } else {
                          if (features[26] < 0.017932f) {
                              return -0.246418f;
                          } else {
                              return 0.365556f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 5.651219f) {
                  if (features[16] < 0.673115f) {
                      if (features[21] < -2.569112f) {
                          return 1.781818f;
                      } else {
                          if (features[26] < 0.151888f) {
                              return 0.738261f;
                          } else {
                              return 0.057131f;
                          }
                      }
                  } else {
                      if (features[22] < 3.560790f) {
                          if (features[7] < 0.085597f) {
                              return -0.167326f;
                          } else {
                              return 0.149665f;
                          }
                      } else {
                          if (features[22] < 4.648651f) {
                              return -0.547655f;
                          } else {
                              return -1.515445f;
                          }
                      }
                  }
              } else {
                  if (features[6] < 30.300733f) {
                      if (features[26] < -0.738866f) {
                          if (features[0] < 29.793232f) {
                              return -1.356800f;
                          } else {
                              return -0.200556f;
                          }
                      } else {
                          if (features[23] < 0.407640f) {
                              return -9.855238f;
                          } else {
                              return -5.564400f;
                          }
                      }
                  } else {
                      if (features[8] < 59.058898f) {
                          if (features[0] < 35.135236f) {
                              return -0.705172f;
                          } else {
                              return 3.822331f;
                          }
                      } else {
                          if (features[4] < 0.218825f) {
                              return -2.217886f;
                          } else {
                              return -1.116371f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_2(const float *features, int32_t features_length) {
          if (features[25] < -0.139243f) {
              if (features[22] < 3.770285f) {
                  if (features[6] < 30.143125f) {
                      if (features[6] < 29.215994f) {
                          if (features[21] < 0.703240f) {
                              return 0.184728f;
                          } else {
                              return -0.840284f;
                          }
                      } else {
                          if (features[26] < 0.416089f) {
                              return 2.761667f;
                          } else {
                              return 0.404904f;
                          }
                      }
                  } else {
                      if (features[22] < 1.098482f) {
                          if (features[22] < 0.004430f) {
                              return 2.824000f;
                          } else {
                              return 1.580000f;
                          }
                      } else {
                          if (features[21] < -1.047464f) {
                              return 0.056040f;
                          } else {
                              return 1.055537f;
                          }
                      }
                  }
              } else {
                  if (features[19] < -0.574401f) {
                      if (features[21] < -0.862428f) {
                          if (features[2] < -0.352562f) {
                              return 5.008954f;
                          } else {
                              return 2.721210f;
                          }
                      } else {
                          return 0.044057f;
                      }
                  } else {
                      if (features[4] < -0.325630f) {
                          if (features[26] < -0.732811f) {
                              return 1.315833f;
                          } else {
                              return 2.657847f;
                          }
                      } else {
                          if (features[14] < 72.427609f) {
                              return 0.173249f;
                          } else {
                              return 0.988305f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 3.968577f) {
                  if (features[26] < -0.740631f) {
                      if (features[16] < 3.633742f) {
                          return 2.174148f;
                      } else {
                          return 1.639333f;
                      }
                  } else {
                      if (features[26] < -0.119374f) {
                          if (features[4] < 0.109026f) {
                              return -0.202298f;
                          } else {
                              return -0.726345f;
                          }
                      } else {
                          if (features[6] < 29.119232f) {
                              return 0.065415f;
                          } else {
                              return 0.229796f;
                          }
                      }
                  }
              } else {
                  if (features[15] < 1.576836f) {
                      if (features[25] < 0.781807f) {
                          if (features[19] < 0.522264f) {
                              return -0.467301f;
                          } else {
                              return -1.694669f;
                          }
                      } else {
                          if (features[16] < 5.022609f) {
                              return -1.283174f;
                          } else {
                              return -5.676923f;
                          }
                      }
                  } else {
                      if (features[6] < 33.505183f) {
                          if (features[0] < 31.982386f) {
                              return -7.855628f;
                          } else {
                              return -2.037600f;
                          }
                      } else {
                          return -0.302639f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_3(const float *features, int32_t features_length) {
          if (features[12] < -5.050486f) {
              if (features[8] < 74.614410f) {
                  if (features[13] < -13.003765f) {
                      return -4.518500f;
                  } else {
                      return 0.245556f;
                  }
              } else {
                  return -6.896955f;
              }
          } else {
              if (features[25] < -0.587259f) {
                  if (features[16] < 4.914905f) {
                      if (features[26] < 0.041635f) {
                          if (features[5] < -2.038297f) {
                              return 1.265694f;
                          } else {
                              return 0.033086f;
                          }
                      } else {
                          if (features[6] < 28.963072f) {
                              return 0.066395f;
                          } else {
                              return 1.231601f;
                          }
                      }
                  } else {
                      if (features[3] < -0.242146f) {
                          if (features[23] < 0.415342f) {
                              return 3.378889f;
                          } else {
                              return 4.846111f;
                          }
                      } else {
                          if (features[2] < -0.003244f) {
                              return 2.478387f;
                          } else {
                              return 0.795500f;
                          }
                      }
                  }
              } else {
                  if (features[4] < 0.012514f) {
                      if (features[7] < 0.492489f) {
                          if (features[12] < 5.353754f) {
                              return 0.207842f;
                          } else {
                              return 4.466275f;
                          }
                      } else {
                          return -2.578929f;
                      }
                  } else {
                      if (features[25] < -0.046577f) {
                          if (features[8] < 72.945142f) {
                              return -0.038931f;
                          } else {
                              return 1.169039f;
                          }
                      } else {
                          if (features[21] < 0.451183f) {
                              return -0.162226f;
                          } else {
                              return -1.387499f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_4(const float *features, int32_t features_length) {
          if (features[25] < 0.541385f) {
              if (features[25] < 0.152315f) {
                  if (features[2] < -0.223445f) {
                      if (features[8] < 81.301235f) {
                          if (features[9] < 0.956250f) {
                              return 2.111814f;
                          } else {
                              return 4.139391f;
                          }
                      } else {
                          if (features[25] < -0.697825f) {
                              return 0.048542f;
                          } else {
                              return 1.122791f;
                          }
                      }
                  } else {
                      if (features[25] < 0.071438f) {
                          if (features[5] < -0.703636f) {
                              return 1.282166f;
                          } else {
                              return 0.467043f;
                          }
                      } else {
                          if (features[16] < 5.600947f) {
                              return -0.092121f;
                          } else {
                              return -0.957143f;
                          }
                      }
                  }
              } else {
                  if (features[18] < 0.365660f) {
                      if (features[20] < 0.479822f) {
                          if (features[21] < -1.243004f) {
                              return 0.485032f;
                          } else {
                              return -0.191059f;
                          }
                      } else {
                          if (features[1] < 0.021806f) {
                              return -0.244667f;
                          } else {
                              return -1.125873f;
                          }
                      }
                  } else {
                      if (features[1] < 0.068315f) {
                          return -1.153922f;
                      } else {
                          return -2.441250f;
                      }
                  }
              }
          } else {
              if (features[26] < -0.165524f) {
                  if (features[13] < -5.942797f) {
                      if (features[25] < 0.731912f) {
                          return -1.255397f;
                      } else {
                          if (features[8] < 67.652401f) {
                              return -3.393636f;
                          } else {
                              return -8.218182f;
                          }
                      }
                  } else {
                      if (features[14] < 65.721763f) {
                          return 2.289399f;
                      } else {
                          if (features[16] < 3.862229f) {
                              return -0.452599f;
                          } else {
                              return -1.593605f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 2.974075f) {
                      if (features[12] < -0.862507f) {
                          if (features[25] < 0.681807f) {
                              return 0.190000f;
                          } else {
                              return 0.579804f;
                          }
                      } else {
                          if (features[6] < 25.838172f) {
                              return -0.465517f;
                          } else {
                              return 0.123680f;
                          }
                      }
                  } else {
                      if (features[8] < 90.363468f) {
                          return 0.558596f;
                      } else {
                          if (features[3] < 0.002293f) {
                              return -0.647500f;
                          } else {
                              return -2.348636f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_5(const float *features, int32_t features_length) {
          if (features[25] < 0.579987f) {
              if (features[3] < -0.462447f) {
                  if (features[13] < -1.905035f) {
                      return -0.503529f;
                  } else {
                      if (features[0] < 28.803880f) {
                          return 0.477265f;
                      } else {
                          if (features[14] < 76.577077f) {
                              return 3.573011f;
                          } else {
                              return 0.850815f;
                          }
                      }
                  }
              } else {
                  if (features[2] < -0.026577f) {
                      if (features[4] < -0.381578f) {
                          if (features[22] < 4.430696f) {
                              return 1.015247f;
                          } else {
                              return 3.111429f;
                          }
                      } else {
                          if (features[0] < 26.668706f) {
                              return -0.183387f;
                          } else {
                              return 0.633568f;
                          }
                      }
                  } else {
                      if (features[20] < 0.712228f) {
                          if (features[16] < 6.264837f) {
                              return 0.179848f;
                          } else {
                              return -0.675505f;
                          }
                      } else {
                          if (features[25] < -0.178072f) {
                              return -0.066774f;
                          } else {
                              return -1.157151f;
                          }
                      }
                  }
              }
          } else {
              if (features[22] < 3.904201f) {
                  if (features[16] < 3.263052f) {
                      if (features[6] < 29.124024f) {
                          if (features[20] < 0.876201f) {
                              return 0.081724f;
                          } else {
                              return -0.367556f;
                          }
                      } else {
                          if (features[6] < 30.077864f) {
                              return 0.081273f;
                          } else {
                              return 0.346006f;
                          }
                      }
                  } else {
                      if (features[3] < 0.201364f) {
                          if (features[6] < 26.947956f) {
                              return 0.151111f;
                          } else {
                              return -0.554948f;
                          }
                      } else {
                          return -1.005769f;
                      }
                  }
              } else {
                  if (features[11] < -2.487357f) {
                      if (features[14] < 77.737483f) {
                          return -2.497786f;
                      } else {
                          return -7.395556f;
                      }
                  } else {
                      if (features[6] < 34.303838f) {
                          if (features[16] < 7.127627f) {
                              return -1.796847f;
                          } else {
                              return -6.124444f;
                          }
                      } else {
                          return 2.912952f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_6(const float *features, int32_t features_length) {
          if (features[13] < -7.038508f) {
              if (features[25] < 0.832368f) {
                  if (features[0] < 32.930790f) {
                      return -3.720000f;
                  } else {
                      if (features[13] < -16.937307f) {
                          return -2.610667f;
                      } else {
                          return 1.090714f;
                      }
                  }
              } else {
                  return -8.138571f;
              }
          } else {
              if (features[25] < 0.736914f) {
                  if (features[13] < 4.188994f) {
                      if (features[25] < -0.160116f) {
                          if (features[14] < 94.735745f) {
                              return 0.815652f;
                          } else {
                              return -0.018925f;
                          }
                      } else {
                          if (features[8] < 62.097545f) {
                              return 1.292542f;
                          } else {
                              return -0.258593f;
                          }
                      }
                  } else {
                      if (features[8] < 88.923428f) {
                          if (features[22] < 4.473869f) {
                              return 1.034315f;
                          } else {
                              return 2.563608f;
                          }
                      } else {
                          if (features[16] < 3.259675f) {
                              return 0.814960f;
                          } else {
                              return -0.774211f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 3.909348f) {
                      if (features[22] < 1.870854f) {
                          if (features[0] < 28.957355f) {
                              return 0.013020f;
                          } else {
                              return 0.300955f;
                          }
                      } else {
                          if (features[14] < 69.287748f) {
                              return 0.639556f;
                          } else {
                              return -0.424781f;
                          }
                      }
                  } else {
                      if (features[22] < 4.825701f) {
                          if (features[12] < -1.700790f) {
                              return -2.172414f;
                          } else {
                              return -0.986123f;
                          }
                      } else {
                          if (features[13] < -0.502472f) {
                              return -6.071000f;
                          } else {
                              return -2.737647f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_7(const float *features, int32_t features_length) {
          if (features[25] < -0.740163f) {
              if (features[22] < 5.259499f) {
                  if (features[0] < 28.116290f) {
                      if (features[15] < 1.500171f) {
                          if (features[0] < 27.793736f) {
                              return 0.034152f;
                          } else {
                              return 0.999630f;
                          }
                      } else {
                          return -2.535764f;
                      }
                  } else {
                      if (features[22] < 3.185782f) {
                          if (features[0] < 29.527598f) {
                              return -0.122641f;
                          } else {
                              return 1.424171f;
                          }
                      } else {
                          if (features[6] < 30.738271f) {
                              return -0.275833f;
                          } else {
                              return 0.518543f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 7.608575f) {
                      if (features[7] < 0.313311f) {
                          return 2.957931f;
                      } else {
                          return 4.930148f;
                      }
                  } else {
                      if (features[26] < -0.417645f) {
                          return 3.526574f;
                      } else {
                          return 0.748125f;
                      }
                  }
              }
          } else {
              if (features[21] < 0.264143f) {
                  if (features[25] < -0.426079f) {
                      if (features[21] < -1.327390f) {
                          return 2.815000f;
                      } else {
                          if (features[26] < 0.618099f) {
                              return 1.208787f;
                          } else {
                              return 0.253895f;
                          }
                      }
                  } else {
                      if (features[8] < 57.725192f) {
                          if (features[7] < 0.094530f) {
                              return 0.292000f;
                          } else {
                              return 2.467634f;
                          }
                      } else {
                          if (features[16] < 4.848734f) {
                              return 0.265630f;
                          } else {
                              return -0.966820f;
                          }
                      }
                  }
              } else {
                  if (features[12] < -3.384689f) {
                      if (features[26] < -0.692974f) {
                          if (features[16] < 8.958926f) {
                              return -1.887669f;
                          } else {
                              return 2.282667f;
                          }
                      } else {
                          if (features[22] < 5.201634f) {
                              return -1.963810f;
                          } else {
                              return -8.456053f;
                          }
                      }
                  } else {
                      if (features[8] < 84.152777f) {
                          if (features[11] < 1.329127f) {
                              return -0.597550f;
                          } else {
                              return 0.850199f;
                          }
                      } else {
                          if (features[6] < 26.551216f) {
                              return -0.608000f;
                          } else {
                              return -1.823654f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_8(const float *features, int32_t features_length) {
          if (features[25] < 0.140360f) {
              if (features[6] < 28.766148f) {
                  if (features[26] < -0.628251f) {
                      if (features[14] < 95.453713f) {
                          if (features[21] < -0.676648f) {
                              return 2.535128f;
                          } else {
                              return 0.662073f;
                          }
                      } else {
                          return -0.278788f;
                      }
                  } else {
                      if (features[13] < 4.293520f) {
                          if (features[0] < 26.316267f) {
                              return -0.484615f;
                          } else {
                              return 0.177162f;
                          }
                      } else {
                          return -1.204837f;
                      }
                  }
              } else {
                  if (features[25] < -0.815311f) {
                      if (features[22] < 5.847313f) {
                          if (features[5] < 0.034359f) {
                              return 1.317164f;
                          } else {
                              return 0.323040f;
                          }
                      } else {
                          if (features[25] < -0.869790f) {
                              return 2.300046f;
                          } else {
                              return 5.724000f;
                          }
                      }
                  } else {
                      if (features[12] < 6.289203f) {
                          if (features[1] < -0.148531f) {
                              return 2.085161f;
                          } else {
                              return 0.375149f;
                          }
                      } else {
                          return 3.609935f;
                      }
                  }
              }
          } else {
              if (features[16] < 3.960875f) {
                  if (features[21] < -2.442287f) {
                      if (features[0] < 30.100092f) {
                          return 0.010526f;
                      } else {
                          return 2.639412f;
                      }
                  } else {
                      if (features[1] < -0.094274f) {
                          if (features[8] < 81.831859f) {
                              return 0.602107f;
                          } else {
                              return -0.065217f;
                          }
                      } else {
                          if (features[26] < -0.544137f) {
                              return 0.835185f;
                          } else {
                              return -0.045631f;
                          }
                      }
                  }
              } else {
                  if (features[11] < -2.656425f) {
                      if (features[0] < 31.219905f) {
                          return -7.596071f;
                      } else {
                          if (features[0] < 33.653128f) {
                              return -2.893889f;
                          } else {
                              return -0.078000f;
                          }
                      }
                  } else {
                      if (features[14] < 57.334751f) {
                          return 2.691276f;
                      } else {
                          if (features[15] < 1.118396f) {
                              return -1.315520f;
                          } else {
                              return -3.037027f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_9(const float *features, int32_t features_length) {
          if (features[25] < -0.607014f) {
              if (features[6] < 29.079358f) {
                  if (features[11] < 2.220380f) {
                      if (features[25] < -0.959206f) {
                          if (features[16] < 2.499267f) {
                              return 1.616000f;
                          } else {
                              return -0.781259f;
                          }
                      } else {
                          if (features[6] < 27.041846f) {
                              return -0.321881f;
                          } else {
                              return 0.492620f;
                          }
                      }
                  } else {
                      return -0.853182f;
                  }
              } else {
                  if (features[22] < 5.993467f) {
                      if (features[26] < -0.380706f) {
                          if (features[21] < -1.192790f) {
                              return 1.715217f;
                          } else {
                              return -0.165271f;
                          }
                      } else {
                          if (features[25] < -0.787120f) {
                              return 1.340362f;
                          } else {
                              return 0.240686f;
                          }
                      }
                  } else {
                      if (features[21] < -1.691562f) {
                          return 4.874248f;
                      } else {
                          if (features[26] < -0.468703f) {
                              return 3.304505f;
                          } else {
                              return 1.680313f;
                          }
                      }
                  }
              }
          } else {
              if (features[12] < -3.536095f) {
                  if (features[14] < 74.382253f) {
                      if (features[5] < 4.373442f) {
                          if (features[7] < 0.181319f) {
                              return -1.219206f;
                          } else {
                              return 1.076957f;
                          }
                      } else {
                          return -3.826250f;
                      }
                  } else {
                      if (features[7] < 0.494343f) {
                          if (features[12] < -4.645407f) {
                              return -10.889333f;
                          } else {
                              return -4.628693f;
                          }
                      } else {
                          return -2.990556f;
                      }
                  }
              } else {
                  if (features[3] < 0.021798f) {
                      if (features[7] < 0.402504f) {
                          if (features[2] < -0.270189f) {
                              return 2.732286f;
                          } else {
                              return 0.203085f;
                          }
                      } else {
                          return -2.547845f;
                      }
                  } else {
                      if (features[21] < 0.549838f) {
                          if (features[16] < 4.477889f) {
                              return 0.136475f;
                          } else {
                              return -0.568395f;
                          }
                      } else {
                          if (features[6] < 29.117228f) {
                              return -1.624403f;
                          } else {
                              return -0.921205f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_10(const float *features, int32_t features_length) {
          if (features[13] < -4.270098f) {
              if (features[6] < 34.323505f) {
                  if (features[25] < 0.392031f) {
                      if (features[17] < -0.036177f) {
                          return 0.821379f;
                      } else {
                          return -1.238724f;
                      }
                  } else {
                      if (features[15] < 1.504198f) {
                          if (features[0] < 30.779589f) {
                              return -3.284468f;
                          } else {
                              return -1.014348f;
                          }
                      } else {
                          if (features[0] < 30.631911f) {
                              return -9.504082f;
                          } else {
                              return -2.980333f;
                          }
                      }
                  }
              } else {
                  return 1.063492f;
              }
          } else {
              if (features[25] < -0.199790f) {
                  if (features[14] < 87.689170f) {
                      if (features[12] < 3.070119f) {
                          if (features[18] < -0.564145f) {
                              return 2.129919f;
                          } else {
                              return 0.761766f;
                          }
                      } else {
                          if (features[8] < 85.001310f) {
                              return 2.356512f;
                          } else {
                              return 1.078846f;
                          }
                      }
                  } else {
                      if (features[25] < -0.264266f) {
                          if (features[20] < 0.020045f) {
                              return 0.367080f;
                          } else {
                              return -0.301102f;
                          }
                      } else {
                          return 2.131667f;
                      }
                  }
              } else {
                  if (features[16] < 3.444998f) {
                      if (features[15] < 1.163957f) {
                          if (features[8] < 67.275982f) {
                              return 0.484773f;
                          } else {
                              return 0.063946f;
                          }
                      } else {
                          if (features[14] < 70.789101f) {
                              return 1.379514f;
                          } else {
                              return 0.459514f;
                          }
                      }
                  } else {
                      if (features[14] < 85.780498f) {
                          if (features[25] < -0.043517f) {
                              return 0.528667f;
                          } else {
                              return -0.692194f;
                          }
                      } else {
                          if (features[26] < -0.740092f) {
                              return -0.522805f;
                          } else {
                              return -2.094530f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_11(const float *features, int32_t features_length) {
          if (features[25] < -0.605641f) {
              if (features[6] < 30.584117f) {
                  if (features[21] < -2.211136f) {
                      return 2.731538f;
                  } else {
                      if (features[22] < 2.205182f) {
                          if (features[14] < 94.400201f) {
                              return 1.442235f;
                          } else {
                              return -0.634054f;
                          }
                      } else {
                          if (features[25] < -0.767376f) {
                              return -0.270823f;
                          } else {
                              return 0.422795f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 5.391114f) {
                      if (features[26] < -0.680090f) {
                          return -0.478261f;
                      } else {
                          if (features[5] < -0.450752f) {
                              return 1.799409f;
                          } else {
                              return 0.812325f;
                          }
                      }
                  } else {
                      if (features[2] < -0.164091f) {
                          if (features[6] < 33.883511f) {
                              return 4.522727f;
                          } else {
                              return 3.460236f;
                          }
                      } else {
                          if (features[16] < 8.324642f) {
                              return 1.322121f;
                          } else {
                              return 3.922667f;
                          }
                      }
                  }
              }
          } else {
              if (features[12] < -4.067553f) {
                  if (features[25] < 0.742184f) {
                      if (features[22] < 8.597528f) {
                          return -2.242886f;
                      } else {
                          return 0.593182f;
                      }
                  } else {
                      if (features[26] < -0.413303f) {
                          return -4.403200f;
                      } else {
                          return -9.375333f;
                      }
                  }
              } else {
                  if (features[21] < 0.291201f) {
                      if (features[25] < 0.265042f) {
                          if (features[22] < 7.322710f) {
                              return 0.492731f;
                          } else {
                              return -1.504194f;
                          }
                      } else {
                          if (features[0] < 33.950503f) {
                              return -0.005876f;
                          } else {
                              return 2.166012f;
                          }
                      }
                  } else {
                      if (features[3] < 0.026847f) {
                          if (features[14] < 72.193711f) {
                              return 0.549800f;
                          } else {
                              return -0.560686f;
                          }
                      } else {
                          if (features[8] < 73.846566f) {
                              return -0.670247f;
                          } else {
                              return -1.810400f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_12(const float *features, int32_t features_length) {
          if (features[4] < 0.208260f) {
              if (features[6] < 29.944510f) {
                  if (features[26] < -0.624496f) {
                      if (features[6] < 27.416248f) {
                          if (features[20] < -0.065826f) {
                              return 0.143939f;
                          } else {
                              return -0.835741f;
                          }
                      } else {
                          if (features[21] < -0.430595f) {
                              return 3.016750f;
                          } else {
                              return 0.497024f;
                          }
                      }
                  } else {
                      if (features[22] < 4.675682f) {
                          if (features[22] < 3.717433f) {
                              return 0.072410f;
                          } else {
                              return -0.510408f;
                          }
                      } else {
                          if (features[11] < -0.368735f) {
                              return -5.752500f;
                          } else {
                              return -3.544000f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.624187f) {
                      if (features[22] < 6.324555f) {
                          if (features[22] < 0.328815f) {
                              return 2.649667f;
                          } else {
                              return 0.870510f;
                          }
                      } else {
                          if (features[25] < -0.899596f) {
                              return 2.145333f;
                          } else {
                              return 4.145926f;
                          }
                      }
                  } else {
                      if (features[25] < -0.135239f) {
                          if (features[18] < -0.593046f) {
                              return 3.295661f;
                          } else {
                              return 0.776000f;
                          }
                      } else {
                          if (features[22] < 3.607006f) {
                              return 0.259666f;
                          } else {
                              return -0.885960f;
                          }
                      }
                  }
              }
          } else {
              if (features[21] < 1.022297f) {
                  if (features[25] < 0.087768f) {
                      if (features[11] < -0.191198f) {
                          if (features[25] < -0.751604f) {
                              return -0.177662f;
                          } else {
                              return 0.876407f;
                          }
                      } else {
                          if (features[21] < -0.570961f) {
                              return 0.420769f;
                          } else {
                              return -0.571475f;
                          }
                      }
                  } else {
                      if (features[14] < 59.534053f) {
                          return 2.266845f;
                      } else {
                          if (features[12] < -3.915451f) {
                              return -3.144242f;
                          } else {
                              return -0.900685f;
                          }
                      }
                  }
              } else {
                  if (features[12] < -7.514499f) {
                      return -6.361053f;
                  } else {
                      if (features[16] < 5.711627f) {
                          if (features[26] < -0.275310f) {
                              return -1.144746f;
                          } else {
                              return 0.154783f;
                          }
                      } else {
                          if (features[6] < 29.068850f) {
                              return -5.888125f;
                          } else {
                              return -1.500483f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_13(const float *features, int32_t features_length) {
          if (features[25] < 0.353330f) {
              if (features[12] < 2.332019f) {
                  if (features[25] < -0.071263f) {
                      if (features[22] < 7.016880f) {
                          if (features[8] < 95.169562f) {
                              return 0.692946f;
                          } else {
                              return -0.048515f;
                          }
                      } else {
                          if (features[4] < 0.059793f) {
                              return 3.360000f;
                          } else {
                              return 0.599286f;
                          }
                      }
                  } else {
                      if (features[18] < 0.511059f) {
                          if (features[25] < 0.097664f) {
                              return 0.398077f;
                          } else {
                              return -0.349411f;
                          }
                      } else {
                          if (features[25] < 0.179285f) {
                              return -1.611250f;
                          } else {
                              return -2.803922f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 29.345496f) {
                      if (features[26] < -0.627838f) {
                          if (features[23] < 0.234267f) {
                              return 1.975956f;
                          } else {
                              return 0.928642f;
                          }
                      } else {
                          if (features[16] < 0.147893f) {
                              return 0.389568f;
                          } else {
                              return -1.352159f;
                          }
                      }
                  } else {
                      if (features[17] < -0.191959f) {
                          if (features[14] < 76.916739f) {
                              return 3.034333f;
                          } else {
                              return 2.505789f;
                          }
                      } else {
                          if (features[4] < -0.062906f) {
                              return 1.720032f;
                          } else {
                              return -0.376667f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 2.186723f) {
                  if (features[21] < -0.737700f) {
                      return 1.621875f;
                  } else {
                      if (features[10] < -0.337111f) {
                          if (features[8] < 95.644545f) {
                              return 0.352119f;
                          } else {
                              return -0.220667f;
                          }
                      } else {
                          if (features[6] < 25.686139f) {
                              return -0.411250f;
                          } else {
                              return 0.078929f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -8.918712f) {
                      if (features[25] < 0.829690f) {
                          if (features[0] < 34.071024f) {
                              return -3.732500f;
                          } else {
                              return -0.621053f;
                          }
                      } else {
                          return -9.688125f;
                      }
                  } else {
                      if (features[15] < 1.203585f) {
                          if (features[16] < 4.048098f) {
                              return -0.091469f;
                          } else {
                              return -1.260968f;
                          }
                      } else {
                          if (features[16] < 4.736864f) {
                              return -0.179894f;
                          } else {
                              return -4.323632f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_14(const float *features, int32_t features_length) {
          if (features[25] < 0.758297f) {
              if (features[1] < 0.042413f) {
                  if (features[0] < 31.861176f) {
                      if (features[25] < 0.240132f) {
                          if (features[4] < -0.297556f) {
                              return 1.225977f;
                          } else {
                              return 0.429466f;
                          }
                      } else {
                          if (features[22] < 5.077435f) {
                              return -0.003373f;
                          } else {
                              return -1.537015f;
                          }
                      }
                  } else {
                      if (features[13] < 4.683000f) {
                          if (features[22] < 7.719169f) {
                              return 0.688935f;
                          } else {
                              return 1.968793f;
                          }
                      } else {
                          if (features[14] < 69.159118f) {
                              return 2.054833f;
                          } else {
                              return 3.579918f;
                          }
                      }
                  }
              } else {
                  if (features[21] < 0.277122f) {
                      if (features[0] < 30.307345f) {
                          if (features[14] < 84.849606f) {
                              return -0.544455f;
                          } else {
                              return 0.044330f;
                          }
                      } else {
                          if (features[17] < 0.179237f) {
                              return 0.448839f;
                          } else {
                              return -1.565238f;
                          }
                      }
                  } else {
                      if (features[25] < -0.172494f) {
                          if (features[9] < -0.254150f) {
                              return 0.244503f;
                          } else {
                              return -0.433175f;
                          }
                      } else {
                          if (features[14] < 65.906962f) {
                              return 0.345652f;
                          } else {
                              return -1.677811f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 4.598351f) {
                  if (features[22] < 1.644118f) {
                      if (features[8] < 96.155374f) {
                          if (features[13] < 0.448391f) {
                              return 0.404295f;
                          } else {
                              return 0.084006f;
                          }
                      } else {
                          if (features[3] < 0.040691f) {
                              return -0.065424f;
                          } else {
                              return -0.469524f;
                          }
                      }
                  } else {
                      if (features[16] < 3.950416f) {
                          if (features[26] < -0.168223f) {
                              return -0.485430f;
                          } else {
                              return 0.207238f;
                          }
                      } else {
                          if (features[5] < 0.611367f) {
                              return -0.762125f;
                          } else {
                              return -2.387895f;
                          }
                      }
                  }
              } else {
                  if (features[15] < 1.267567f) {
                      if (features[10] < -0.360567f) {
                          if (features[8] < 81.979046f) {
                              return -2.076250f;
                          } else {
                              return -4.681667f;
                          }
                      } else {
                          if (features[23] < 0.234469f) {
                              return -3.238947f;
                          } else {
                              return -2.266400f;
                          }
                      }
                  } else {
                      if (features[5] < 0.809645f) {
                          return -7.910000f;
                      } else {
                          return -6.707647f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_15(const float *features, int32_t features_length) {
          if (features[25] < -0.188859f) {
              if (features[22] < 4.951686f) {
                  if (features[8] < 90.829744f) {
                      if (features[10] < 3.134006f) {
                          if (features[22] < 1.847772f) {
                              return 1.534500f;
                          } else {
                              return 0.641865f;
                          }
                      } else {
                          return 3.536593f;
                      }
                  } else {
                      if (features[6] < 26.796709f) {
                          if (features[16] < 3.450798f) {
                              return 0.102041f;
                          } else {
                              return -0.920000f;
                          }
                      } else {
                          if (features[14] < 94.143206f) {
                              return 0.712783f;
                          } else {
                              return -0.003665f;
                          }
                      }
                  }
              } else {
                  if (features[3] < -0.169965f) {
                      if (features[14] < 73.447880f) {
                          if (features[15] < 0.667126f) {
                              return 0.534118f;
                          } else {
                              return 3.056167f;
                          }
                      } else {
                          return 4.331875f;
                      }
                  } else {
                      if (features[1] < 0.175823f) {
                          if (features[14] < 68.101826f) {
                              return 0.264287f;
                          } else {
                              return 2.179744f;
                          }
                      } else {
                          return -0.660556f;
                      }
                  }
              }
          } else {
              if (features[21] < 0.566905f) {
                  if (features[0] < 35.149926f) {
                      if (features[16] < 8.728981f) {
                          if (features[16] < 4.808157f) {
                              return 0.122913f;
                          } else {
                              return -0.945528f;
                          }
                      } else {
                          return -1.826667f;
                      }
                  } else {
                      return 3.165238f;
                  }
              } else {
                  if (features[16] < 7.648712f) {
                      if (features[22] < 4.096303f) {
                          if (features[14] < 75.696512f) {
                              return 0.231212f;
                          } else {
                              return -0.625034f;
                          }
                      } else {
                          if (features[14] < 75.662804f) {
                              return -1.111693f;
                          } else {
                              return -2.756684f;
                          }
                      }
                  } else {
                      if (features[6] < 29.735830f) {
                          return -9.820526f;
                      } else {
                          if (features[21] < 1.764934f) {
                              return -0.084000f;
                          } else {
                              return -3.201875f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_16(const float *features, int32_t features_length) {
          if (features[25] < -0.039630f) {
              if (features[3] < -0.497341f) {
                  if (features[8] < 82.918721f) {
                      if (features[8] < 71.954540f) {
                          return 3.765000f;
                      } else {
                          return 2.516605f;
                      }
                  } else {
                      if (features[25] < -0.773640f) {
                          return 0.763529f;
                      } else {
                          return 1.506667f;
                      }
                  }
              } else {
                  if (features[6] < 29.411499f) {
                      if (features[25] < -0.471635f) {
                          if (features[26] < -0.365265f) {
                              return 0.592648f;
                          } else {
                              return -0.182742f;
                          }
                      } else {
                          if (features[8] < 95.665801f) {
                              return 0.958735f;
                          } else {
                              return -0.121786f;
                          }
                      }
                  } else {
                      if (features[12] < 5.983543f) {
                          if (features[4] < -0.431652f) {
                              return 2.360974f;
                          } else {
                              return 0.727584f;
                          }
                      } else {
                          return 3.247500f;
                      }
                  }
              }
          } else {
              if (features[22] < 3.205728f) {
                  if (features[25] < 0.054565f) {
                      if (features[6] < 28.980340f) {
                          return 0.537143f;
                      } else {
                          return 0.962000f;
                      }
                  } else {
                      if (features[0] < 26.931766f) {
                          if (features[14] < 90.454884f) {
                              return -0.720000f;
                          } else {
                              return -0.032706f;
                          }
                      } else {
                          if (features[22] < 2.183414f) {
                              return 0.178057f;
                          } else {
                              return -0.190426f;
                          }
                      }
                  }
              } else {
                  if (features[24] < 0.200101f) {
                      if (features[16] < 7.735953f) {
                          if (features[25] < 0.625854f) {
                              return -0.367662f;
                          } else {
                              return -1.654111f;
                          }
                      } else {
                          if (features[26] < -0.725576f) {
                              return -2.594167f;
                          } else {
                              return -7.409355f;
                          }
                      }
                  } else {
                      return 4.036452f;
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_17(const float *features, int32_t features_length) {
          if (features[25] < -0.093686f) {
              if (features[14] < 91.871229f) {
                  if (features[1] < -0.022960f) {
                      if (features[11] < 3.328050f) {
                          if (features[22] < 6.489482f) {
                              return 1.108181f;
                          } else {
                              return 3.016166f;
                          }
                      } else {
                          if (features[14] < 71.030699f) {
                              return 5.256667f;
                          } else {
                              return 2.273935f;
                          }
                      }
                  } else {
                      if (features[12] < 0.329881f) {
                          if (features[22] < 1.893450f) {
                              return 2.060000f;
                          } else {
                              return 0.661442f;
                          }
                      } else {
                          if (features[13] < 5.033231f) {
                              return 0.051605f;
                          } else {
                              return 1.020983f;
                          }
                      }
                  }
              } else {
                  if (features[14] < 94.413116f) {
                      if (features[22] < 2.230288f) {
                          if (features[22] < 1.236986f) {
                              return 0.324286f;
                          } else {
                              return 2.625000f;
                          }
                      } else {
                          if (features[7] < 0.121485f) {
                              return -0.485000f;
                          } else {
                              return 0.435000f;
                          }
                      }
                  } else {
                      if (features[11] < 1.371328f) {
                          if (features[8] < 97.194928f) {
                              return 0.076222f;
                          } else {
                              return -0.228704f;
                          }
                      } else {
                          return -0.604118f;
                      }
                  }
              }
          } else {
              if (features[16] < 3.560191f) {
                  if (features[21] < -2.416585f) {
                      if (features[0] < 30.351210f) {
                          return 0.055000f;
                      } else {
                          return 2.870667f;
                      }
                  } else {
                      if (features[25] < 0.028585f) {
                          if (features[8] < 80.044589f) {
                              return 0.205000f;
                          } else {
                              return 0.816349f;
                          }
                      } else {
                          if (features[5] < -0.540336f) {
                              return -0.633333f;
                          } else {
                              return 0.055940f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 4.322999f) {
                      if (features[25] < 0.768231f) {
                          if (features[0] < 29.786755f) {
                              return 1.111957f;
                          } else {
                              return -0.905333f;
                          }
                      } else {
                          if (features[4] < 0.008187f) {
                              return 0.048998f;
                          } else {
                              return -0.986480f;
                          }
                      }
                  } else {
                      if (features[26] < -0.595764f) {
                          if (features[24] < 0.088714f) {
                              return -1.250861f;
                          } else {
                              return 4.036452f;
                          }
                      } else {
                          if (features[22] < 4.680705f) {
                              return -1.585707f;
                          } else {
                              return -5.516143f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_18(const float *features, int32_t features_length) {
          if (features[13] < -6.315639f) {
              if (features[6] < 33.884838f) {
                  if (features[14] < 79.743988f) {
                      if (features[5] < 2.370081f) {
                          return -1.380435f;
                      } else {
                          return -5.554000f;
                      }
                  } else {
                      if (features[25] < 0.829574f) {
                          return -4.332083f;
                      } else {
                          return -7.053500f;
                      }
                  }
              } else {
                  return 0.154396f;
              }
          } else {
              if (features[25] < 0.277722f) {
                  if (features[1] < -0.075434f) {
                      if (features[0] < 27.239690f) {
                          if (features[0] < 26.545923f) {
                              return -0.340667f;
                          } else {
                              return 0.220689f;
                          }
                      } else {
                          if (features[22] < 3.945815f) {
                              return 1.073494f;
                          } else {
                              return 2.374151f;
                          }
                      }
                  } else {
                      if (features[21] < -2.261943f) {
                          if (features[25] < -0.268869f) {
                              return 2.662258f;
                          } else {
                              return 0.187222f;
                          }
                      } else {
                          if (features[19] < 0.688723f) {
                              return 0.344412f;
                          } else {
                              return -0.578596f;
                          }
                      }
                  }
              } else {
                  if (features[18] < 0.210582f) {
                      if (features[22] < 4.232310f) {
                          if (features[21] < -0.632165f) {
                              return 0.720444f;
                          } else {
                              return 0.025739f;
                          }
                      } else {
                          if (features[8] < 62.718441f) {
                              return 1.442222f;
                          } else {
                              return -1.318921f;
                          }
                      }
                  } else {
                      if (features[16] < 4.312249f) {
                          if (features[14] < 75.621390f) {
                              return 0.263667f;
                          } else {
                              return -0.425429f;
                          }
                      } else {
                          if (features[8] < 76.964473f) {
                              return -1.435245f;
                          } else {
                              return -2.869163f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_19(const float *features, int32_t features_length) {
          if (features[25] < -0.340384f) {
              if (features[16] < 6.086155f) {
                  if (features[21] < -1.270551f) {
                      if (features[22] < 4.187887f) {
                          if (features[6] < 30.488761f) {
                              return 1.627879f;
                          } else {
                              return 0.323124f;
                          }
                      } else {
                          if (features[8] < 74.479829f) {
                              return 2.029181f;
                          } else {
                              return 4.343074f;
                          }
                      }
                  } else {
                      if (features[6] < 29.565086f) {
                          if (features[25] < -0.871395f) {
                              return -0.444849f;
                          } else {
                              return 0.308648f;
                          }
                      } else {
                          if (features[16] < 3.387545f) {
                              return 1.053982f;
                          } else {
                              return 0.566084f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.585762f) {
                      if (features[3] < -0.514918f) {
                          return 4.232222f;
                      } else {
                          if (features[22] < 8.385676f) {
                              return 2.037500f;
                          } else {
                              return 3.884000f;
                          }
                      }
                  } else {
                      if (features[13] < -0.276337f) {
                          return -0.081667f;
                      } else {
                          return -1.433333f;
                      }
                  }
              }
          } else {
              if (features[16] < 6.322352f) {
                  if (features[16] < 4.869745f) {
                      if (features[25] < 0.101385f) {
                          if (features[26] < 0.402230f) {
                              return 2.482138f;
                          } else {
                              return 0.447488f;
                          }
                      } else {
                          if (features[22] < 3.119512f) {
                              return 0.073502f;
                          } else {
                              return -0.656123f;
                          }
                      }
                  } else {
                      if (features[26] < -0.785794f) {
                          if (features[0] < 29.763320f) {
                              return -1.068750f;
                          } else {
                              return -0.334611f;
                          }
                      } else {
                          if (features[14] < 80.328684f) {
                              return -1.866697f;
                          } else {
                              return -5.435000f;
                          }
                      }
                  }
              } else {
                  if (features[14] < 73.897619f) {
                      if (features[24] < 0.144509f) {
                          if (features[13] < -6.942896f) {
                              return -2.612000f;
                          } else {
                              return -0.751887f;
                          }
                      } else {
                          return 3.619799f;
                      }
                  } else {
                      if (features[6] < 29.324613f) {
                          return -8.754348f;
                      } else {
                          if (features[3] < 0.380202f) {
                              return -5.256875f;
                          } else {
                              return -0.930000f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_20(const float *features, int32_t features_length) {
          if (features[25] < 0.186426f) {
              if (features[3] < -0.289116f) {
                  if (features[14] < 80.872942f) {
                      if (features[15] < 1.950617f) {
                          if (features[25] < -0.657701f) {
                              return 3.255572f;
                          } else {
                              return 0.908444f;
                          }
                      } else {
                          return 3.669956f;
                      }
                  } else {
                      if (features[21] < -0.968686f) {
                          if (features[26] < -0.521779f) {
                              return 2.266000f;
                          } else {
                              return 0.708000f;
                          }
                      } else {
                          if (features[4] < -0.436783f) {
                              return 0.301389f;
                          } else {
                              return -0.883846f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 29.504409f) {
                      if (features[25] < -0.859371f) {
                          if (features[25] < -0.907014f) {
                              return -0.384800f;
                          } else {
                              return 0.543118f;
                          }
                      } else {
                          if (features[0] < 27.059802f) {
                              return -0.089779f;
                          } else {
                              return 0.684957f;
                          }
                      }
                  } else {
                      if (features[26] < -0.870202f) {
                          if (features[22] < 5.879449f) {
                              return 0.636778f;
                          } else {
                              return -0.256102f;
                          }
                      } else {
                          if (features[26] < 0.672235f) {
                              return 1.121611f;
                          } else {
                              return -0.065516f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 4.681740f) {
                  if (features[3] < 0.013412f) {
                      if (features[25] < 0.560307f) {
                          if (features[12] < 2.210073f) {
                              return 0.161559f;
                          } else {
                              return 1.657611f;
                          }
                      } else {
                          if (features[16] < 3.104811f) {
                              return 0.078071f;
                          } else {
                              return -0.227370f;
                          }
                      }
                  } else {
                      if (features[26] < -0.186330f) {
                          if (features[4] < 0.123636f) {
                              return -0.417143f;
                          } else {
                              return -1.253831f;
                          }
                      } else {
                          if (features[0] < 30.362843f) {
                              return -0.028699f;
                          } else {
                              return 0.519752f;
                          }
                      }
                  }
              } else {
                  if (features[8] < 76.578789f) {
                      if (features[0] < 35.093349f) {
                          if (features[2] < 0.544904f) {
                              return -1.129860f;
                          } else {
                              return -3.857727f;
                          }
                      } else {
                          return 4.677768f;
                      }
                  } else {
                      if (features[15] < 1.687542f) {
                          if (features[22] < 6.630138f) {
                              return -2.880789f;
                          } else {
                              return -6.159333f;
                          }
                      } else {
                          return -6.531852f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_21(const float *features, int32_t features_length) {
          if (features[13] < -5.308973f) {
              if (features[0] < 33.940306f) {
                  if (features[16] < 5.309834f) {
                      if (features[22] < 4.198491f) {
                          return -0.415679f;
                      } else {
                          return -2.556071f;
                      }
                  } else {
                      if (features[0] < 29.530737f) {
                          return -11.420000f;
                      } else {
                          if (features[26] < -0.842620f) {
                              return -1.991250f;
                          } else {
                              return -4.912143f;
                          }
                      }
                  }
              } else {
                  if (features[5] < 2.990124f) {
                      return 2.228487f;
                  } else {
                      return -1.787500f;
                  }
              }
          } else {
              if (features[1] < -0.148958f) {
                  if (features[6] < 32.147944f) {
                      if (features[14] < 79.797934f) {
                          if (features[8] < 73.391053f) {
                              return 3.065326f;
                          } else {
                              return 0.806784f;
                          }
                      } else {
                          if (features[21] < 0.254700f) {
                              return 0.493143f;
                          } else {
                              return -0.615000f;
                          }
                      }
                  } else {
                      if (features[14] < 71.601121f) {
                          if (features[26] < -0.584368f) {
                              return 2.013812f;
                          } else {
                              return 3.712667f;
                          }
                      } else {
                          return 4.695000f;
                      }
                  }
              } else {
                  if (features[25] < 0.381808f) {
                      if (features[25] < 0.056408f) {
                          if (features[21] < -2.474332f) {
                              return 2.400000f;
                          } else {
                              return 0.550652f;
                          }
                      } else {
                          if (features[16] < 6.217941f) {
                              return -0.268215f;
                          } else {
                              return -1.055652f;
                          }
                      }
                  } else {
                      if (features[22] < 3.161235f) {
                          if (features[8] < 96.254269f) {
                              return 0.169598f;
                          } else {
                              return -0.117654f;
                          }
                      } else {
                          if (features[14] < 69.620862f) {
                              return -0.584558f;
                          } else {
                              return -1.375221f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_22(const float *features, int32_t features_length) {
          if (features[20] < 0.058396f) {
              if (features[25] < -0.478264f) {
                  if (features[6] < 31.861767f) {
                      if (features[6] < 28.977222f) {
                          if (features[26] < -0.590853f) {
                              return 0.950916f;
                          } else {
                              return -0.040766f;
                          }
                      } else {
                          if (features[5] < -0.289055f) {
                              return 1.522222f;
                          } else {
                              return 0.479957f;
                          }
                      }
                  } else {
                      if (features[14] < 71.368991f) {
                          if (features[5] < -0.606476f) {
                              return 2.742105f;
                          } else {
                              return 0.936171f;
                          }
                      } else {
                          if (features[21] < -0.840028f) {
                              return 7.711875f;
                          } else {
                              return 2.297778f;
                          }
                      }
                  }
              } else {
                  if (features[7] < 0.595793f) {
                      if (features[22] < 9.427023f) {
                          if (features[11] < 2.203859f) {
                              return 0.073923f;
                          } else {
                              return 1.892252f;
                          }
                      } else {
                          return 3.928952f;
                      }
                  } else {
                      return -1.650375f;
                  }
              }
          } else {
              if (features[13] < -1.799012f) {
                  if (features[13] < -6.682910f) {
                      if (features[0] < 33.882965f) {
                          if (features[1] < 0.114069f) {
                              return -4.164615f;
                          } else {
                              return -7.808000f;
                          }
                      } else {
                          return -1.674375f;
                      }
                  } else {
                      if (features[26] < -0.654548f) {
                          if (features[5] < 0.784026f) {
                              return -1.228814f;
                          } else {
                              return -0.127183f;
                          }
                      } else {
                          if (features[8] < 80.914042f) {
                              return -1.071296f;
                          } else {
                              return -3.954000f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.108932f) {
                      if (features[8] < 70.294701f) {
                          if (features[2] < -0.010856f) {
                              return 2.427733f;
                          } else {
                              return 0.055628f;
                          }
                      } else {
                          if (features[2] < -0.215636f) {
                              return -0.866800f;
                          } else {
                              return 0.074758f;
                          }
                      }
                  } else {
                      if (features[16] < 5.416993f) {
                          if (features[16] < 1.747197f) {
                              return 0.570213f;
                          } else {
                              return -0.371355f;
                          }
                      } else {
                          if (features[8] < 69.891774f) {
                              return -1.048000f;
                          } else {
                              return -2.947674f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_23(const float *features, int32_t features_length) {
          if (features[25] < 0.804441f) {
              if (features[21] < 0.362754f) {
                  if (features[25] < -0.047398f) {
                      if (features[1] < -0.288896f) {
                          if (features[23] < 0.279133f) {
                              return 1.845926f;
                          } else {
                              return 4.318538f;
                          }
                      } else {
                          if (features[3] < -0.105464f) {
                              return 1.282993f;
                          } else {
                              return 0.416437f;
                          }
                      }
                  } else {
                      if (features[16] < 4.318750f) {
                          if (features[26] < 0.425141f) {
                              return 1.172704f;
                          } else {
                              return 0.082825f;
                          }
                      } else {
                          if (features[22] < 9.529907f) {
                              return -0.933948f;
                          } else {
                              return 3.526282f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.266244f) {
                      if (features[8] < 63.403240f) {
                          return 2.565357f;
                      } else {
                          if (features[4] < 0.241093f) {
                              return -0.195162f;
                          } else {
                              return 0.998929f;
                          }
                      }
                  } else {
                      if (features[25] < 0.255259f) {
                          if (features[23] < 0.521249f) {
                              return 0.738000f;
                          } else {
                              return -1.376875f;
                          }
                      } else {
                          if (features[20] < 0.519881f) {
                              return -1.291887f;
                          } else {
                              return -2.415414f;
                          }
                      }
                  }
              }
          } else {
              if (features[13] < -3.952086f) {
                  if (features[13] < -9.132698f) {
                      return -8.891111f;
                  } else {
                      if (features[14] < 86.791106f) {
                          if (features[1] < 0.045733f) {
                              return -2.598125f;
                          } else {
                              return -3.108125f;
                          }
                      } else {
                          return -3.963333f;
                      }
                  }
              } else {
                  if (features[26] < -0.075012f) {
                      if (features[14] < 76.137876f) {
                          if (features[14] < 69.185169f) {
                              return 0.475405f;
                          } else {
                              return -0.496887f;
                          }
                      } else {
                          if (features[22] < 4.224473f) {
                              return -0.561687f;
                          } else {
                              return -3.004773f;
                          }
                      }
                  } else {
                      if (features[13] < -0.558384f) {
                          if (features[4] < 0.128914f) {
                              return 0.782222f;
                          } else {
                              return -0.030588f;
                          }
                      } else {
                          if (features[20] < 1.928231f) {
                              return 0.119630f;
                          } else {
                              return -0.578889f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_24(const float *features, int32_t features_length) {
          if (features[25] < 0.090221f) {
              if (features[14] < 88.825697f) {
                  if (features[1] < -0.067958f) {
                      if (features[11] < 4.671440f) {
                          if (features[22] < 6.227722f) {
                              return 1.317635f;
                          } else {
                              return 2.679211f;
                          }
                      } else {
                          return 3.959306f;
                      }
                  } else {
                      if (features[20] < -1.084085f) {
                          if (features[22] < 4.310384f) {
                              return 0.719412f;
                          } else {
                              return 2.124348f;
                          }
                      } else {
                          if (features[18] < 0.522999f) {
                              return 0.514386f;
                          } else {
                              return -0.458095f;
                          }
                      }
                  }
              } else {
                  if (features[20] < 0.312083f) {
                      if (features[26] < -0.947266f) {
                          if (features[26] < -0.990477f) {
                              return -0.068000f;
                          } else {
                              return 4.285000f;
                          }
                      } else {
                          if (features[22] < 4.157431f) {
                              return 0.341174f;
                          } else {
                              return -1.069474f;
                          }
                      }
                  } else {
                      if (features[6] < 29.746711f) {
                          if (features[19] < 0.589379f) {
                              return -0.854444f;
                          } else {
                              return -0.186471f;
                          }
                      } else {
                          return 0.116250f;
                      }
                  }
              }
          } else {
              if (features[16] < 3.634749f) {
                  if (features[8] < 65.634033f) {
                      if (features[8] < 64.922209f) {
                          return 1.911304f;
                      } else {
                          return 0.601200f;
                      }
                  } else {
                      if (features[21] < -2.166588f) {
                          if (features[1] < -0.018684f) {
                              return 1.344000f;
                          } else {
                              return 0.274375f;
                          }
                      } else {
                          if (features[12] < 2.649325f) {
                              return -0.050306f;
                          } else {
                              return 1.129689f;
                          }
                      }
                  }
              } else {
                  if (features[13] < -5.027166f) {
                      if (features[14] < 78.500945f) {
                          if (features[13] < -18.880741f) {
                              return -4.013182f;
                          } else {
                              return -0.639335f;
                          }
                      } else {
                          if (features[5] < 1.263754f) {
                              return -6.846667f;
                          } else {
                              return -2.718125f;
                          }
                      }
                  } else {
                      if (features[15] < 1.389728f) {
                          if (features[6] < 33.839429f) {
                              return -1.047286f;
                          } else {
                              return 1.633333f;
                          }
                      } else {
                          return -3.481563f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_25(const float *features, int32_t features_length) {
          if (features[25] < -0.163211f) {
              if (features[1] < -0.039273f) {
                  if (features[14] < 85.862450f) {
                      if (features[3] < -0.555609f) {
                          if (features[22] < 7.445583f) {
                              return 4.453380f;
                          } else {
                              return 2.416928f;
                          }
                      } else {
                          if (features[0] < 28.118341f) {
                              return -0.326420f;
                          } else {
                              return 1.636426f;
                          }
                      }
                  } else {
                      if (features[20] < 0.011899f) {
                          if (features[26] < -0.909999f) {
                              return 3.241579f;
                          } else {
                              return 0.586026f;
                          }
                      } else {
                          if (features[26] < 0.181331f) {
                              return -0.677727f;
                          } else {
                              return 0.175789f;
                          }
                      }
                  }
              } else {
                  if (features[8] < 90.213899f) {
                      if (features[26] < -0.911650f) {
                          if (features[21] < -0.584362f) {
                              return 0.882762f;
                          } else {
                              return -0.351270f;
                          }
                      } else {
                          if (features[3] < -0.144052f) {
                              return 1.467557f;
                          } else {
                              return 0.551133f;
                          }
                      }
                  } else {
                      if (features[5] < -0.470455f) {
                          if (features[21] < -0.496317f) {
                              return 0.642917f;
                          } else {
                              return -1.334286f;
                          }
                      } else {
                          if (features[14] < 94.141653f) {
                              return 0.755200f;
                          } else {
                              return -0.051760f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 3.164410f) {
                  if (features[6] < 25.965593f) {
                      if (features[4] < -0.057643f) {
                          return -0.350833f;
                      } else {
                          return -0.634400f;
                      }
                  } else {
                      if (features[21] < -1.655389f) {
                          if (features[0] < 29.992264f) {
                              return 0.049767f;
                          } else {
                              return 2.144000f;
                          }
                      } else {
                          if (features[25] < 0.150729f) {
                              return 0.537154f;
                          } else {
                              return 0.084330f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 33.530760f) {
                      if (features[10] < -1.136652f) {
                          if (features[12] < -3.004294f) {
                              return -4.513416f;
                          } else {
                              return -1.187766f;
                          }
                      } else {
                          if (features[16] < 7.051983f) {
                              return -0.787769f;
                          } else {
                              return -4.967768f;
                          }
                      }
                  } else {
                      if (features[5] < 4.072564f) {
                          if (features[22] < 6.509634f) {
                              return -0.211515f;
                          } else {
                              return 2.671008f;
                          }
                      } else {
                          return -2.909375f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_26(const float *features, int32_t features_length) {
          if (features[25] < 0.252041f) {
              if (features[1] < -0.198502f) {
                  if (features[14] < 88.313351f) {
                      if (features[13] < -0.154158f) {
                          return 0.779615f;
                      } else {
                          if (features[14] < 78.801201f) {
                              return 3.633360f;
                          } else {
                              return 0.562000f;
                          }
                      }
                  } else {
                      return 0.040556f;
                  }
              } else {
                  if (features[8] < 90.254855f) {
                      if (features[1] < -0.108847f) {
                          if (features[1] < -0.172547f) {
                              return 3.870370f;
                          } else {
                              return 1.701185f;
                          }
                      } else {
                          if (features[25] < 0.163460f) {
                              return 0.608122f;
                          } else {
                              return -0.398306f;
                          }
                      }
                  } else {
                      if (features[21] < -0.741579f) {
                          if (features[22] < 3.266049f) {
                              return 0.247692f;
                          } else {
                              return 1.531154f;
                          }
                      } else {
                          if (features[26] < -0.062490f) {
                              return -0.447465f;
                          } else {
                              return 0.190518f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 3.581749f) {
                  if (features[14] < 68.655728f) {
                      if (features[8] < 65.693254f) {
                          if (features[16] < 0.112262f) {
                              return 0.838519f;
                          } else {
                              return 2.399412f;
                          }
                      } else {
                          if (features[21] < 0.586306f) {
                              return -0.101444f;
                          } else {
                              return 1.152941f;
                          }
                      }
                  } else {
                      if (features[20] < 0.032944f) {
                          if (features[16] < 3.319745f) {
                              return 0.094536f;
                          } else {
                              return 1.013264f;
                          }
                      } else {
                          if (features[0] < 30.167756f) {
                              return -0.355215f;
                          } else {
                              return 0.117273f;
                          }
                      }
                  }
              } else {
                  if (features[16] < 7.581301f) {
                      if (features[16] < 4.723175f) {
                          if (features[3] < 0.035547f) {
                              return -0.010211f;
                          } else {
                              return -1.176426f;
                          }
                      } else {
                          if (features[13] < -6.483833f) {
                              return -4.750909f;
                          } else {
                              return -1.555848f;
                          }
                      }
                  } else {
                      if (features[6] < 33.217024f) {
                          if (features[26] < -0.524210f) {
                              return -3.797000f;
                          } else {
                              return -9.099091f;
                          }
                      } else {
                          return 2.606905f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_27(const float *features, int32_t features_length) {
          if (features[12] < -3.359279f) {
              if (features[25] < 0.453684f) {
                  if (features[25] < -0.684631f) {
                      return -1.155132f;
                  } else {
                      if (features[25] < 0.034874f) {
                          return 2.025764f;
                      } else {
                          return -0.736875f;
                      }
                  }
              } else {
                  if (features[25] < 0.632777f) {
                      return -0.421048f;
                  } else {
                      if (features[0] < 30.312269f) {
                          if (features[22] < 5.288901f) {
                              return -2.807778f;
                          } else {
                              return -13.500625f;
                          }
                      } else {
                          if (features[21] < 1.917399f) {
                              return -2.335556f;
                          } else {
                              return -4.203750f;
                          }
                      }
                  }
              }
          } else {
              if (features[25] < -0.287294f) {
                  if (features[8] < 91.471322f) {
                      if (features[17] < -0.203694f) {
                          if (features[22] < 5.152019f) {
                              return 1.431594f;
                          } else {
                              return 3.414153f;
                          }
                      } else {
                          if (features[25] < -0.386324f) {
                              return 1.008369f;
                          } else {
                              return -0.327178f;
                          }
                      }
                  } else {
                      if (features[5] < 0.137914f) {
                          if (features[5] < -0.857066f) {
                              return -0.772927f;
                          } else {
                              return 0.075885f;
                          }
                      } else {
                          if (features[20] < -0.092116f) {
                              return 0.697586f;
                          } else {
                              return 0.155625f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 4.480007f) {
                      if (features[25] < 0.031273f) {
                          if (features[26] < -0.724420f) {
                              return 2.123636f;
                          } else {
                              return 0.514339f;
                          }
                      } else {
                          if (features[16] < 4.032257f) {
                              return 0.055025f;
                          } else {
                              return -0.744522f;
                          }
                      }
                  } else {
                      if (features[14] < 62.850215f) {
                          if (features[26] < -0.807926f) {
                              return 0.390033f;
                          } else {
                              return 3.264714f;
                          }
                      } else {
                          if (features[25] < 0.726966f) {
                              return -0.845599f;
                          } else {
                              return -3.041398f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_28(const float *features, int32_t features_length) {
          if (features[25] < -0.384746f) {
              if (features[16] < 5.294329f) {
                  if (features[14] < 91.034066f) {
                      if (features[25] < -0.606179f) {
                          if (features[8] < 90.371082f) {
                              return 0.721622f;
                          } else {
                              return -0.982059f;
                          }
                      } else {
                          if (features[26] < -0.632879f) {
                              return 2.449170f;
                          } else {
                              return 0.520692f;
                          }
                      }
                  } else {
                      if (features[5] < 0.152029f) {
                          if (features[0] < 26.957630f) {
                              return -0.319385f;
                          } else {
                              return 0.346268f;
                          }
                      } else {
                          if (features[10] < -0.351175f) {
                              return 1.090625f;
                          } else {
                              return 0.222800f;
                          }
                      }
                  }
              } else {
                  if (features[3] < -0.019034f) {
                      if (features[0] < 34.018870f) {
                          if (features[23] < 0.236997f) {
                              return 3.266129f;
                          } else {
                              return 5.036889f;
                          }
                      } else {
                          if (features[5] < -0.302195f) {
                              return 2.488571f;
                          } else {
                              return 2.029500f;
                          }
                      }
                  } else {
                      if (features[8] < 64.546169f) {
                          return -0.658947f;
                      } else {
                          return 1.653448f;
                      }
                  }
              }
          } else {
              if (features[22] < 4.594227f) {
                  if (features[1] < -0.058731f) {
                      if (features[25] < 0.224462f) {
                          if (features[26] < 0.547031f) {
                              return 2.591287f;
                          } else {
                              return 0.427912f;
                          }
                      } else {
                          if (features[16] < 4.252908f) {
                              return 0.195829f;
                          } else {
                              return -1.095789f;
                          }
                      }
                  } else {
                      if (features[16] < 1.839156f) {
                          if (features[25] < 0.000876f) {
                              return 0.807460f;
                          } else {
                              return 0.116624f;
                          }
                      } else {
                          if (features[12] < 2.520757f) {
                              return -0.403287f;
                          } else {
                              return 1.909091f;
                          }
                      }
                  }
              } else {
                  if (features[25] < 0.350516f) {
                      if (features[14] < 67.840766f) {
                          if (features[25] < 0.244822f) {
                              return 0.398413f;
                          } else {
                              return -1.096471f;
                          }
                      } else {
                          if (features[1] < 0.092896f) {
                              return -0.475389f;
                          } else {
                              return -1.798654f;
                          }
                      }
                  } else {
                      if (features[14] < 62.240184f) {
                          if (features[14] < 57.489434f) {
                              return 3.716674f;
                          } else {
                              return -0.376471f;
                          }
                      } else {
                          if (features[26] < -0.676134f) {
                              return -1.448439f;
                          } else {
                              return -5.064522f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_29(const float *features, int32_t features_length) {
          if (features[25] < 0.254743f) {
              if (features[21] < -1.087931f) {
                  if (features[22] < 4.359080f) {
                      if (features[0] < 26.850187f) {
                          return -0.130000f;
                      } else {
                          if (features[16] < 2.140759f) {
                              return 0.577705f;
                          } else {
                              return 2.049421f;
                          }
                      }
                  } else {
                      if (features[13] < 0.296082f) {
                          return 1.064167f;
                      } else {
                          if (features[3] < -0.360381f) {
                              return 5.491304f;
                          } else {
                              return 2.820438f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 7.196085f) {
                      if (features[25] < 0.080459f) {
                          if (features[22] < 1.904203f) {
                              return 0.843255f;
                          } else {
                              return 0.367834f;
                          }
                      } else {
                          if (features[22] < 1.789334f) {
                              return 0.219783f;
                          } else {
                              return -0.716208f;
                          }
                      }
                  } else {
                      if (features[8] < 65.878834f) {
                          if (features[4] < -0.054891f) {
                              return 2.565000f;
                          } else {
                              return 1.878235f;
                          }
                      } else {
                          if (features[26] < -0.812956f) {
                              return -0.817333f;
                          } else {
                              return 2.640513f;
                          }
                      }
                  }
              }
          } else {
              if (features[22] < 3.226083f) {
                  if (features[0] < 31.124129f) {
                      if (features[26] < 0.738872f) {
                          if (features[6] < 27.719175f) {
                              return -0.093093f;
                          } else {
                              return 0.226549f;
                          }
                      } else {
                          if (features[14] < 76.632512f) {
                              return -0.391892f;
                          } else {
                              return 0.108314f;
                          }
                      }
                  } else {
                      return 1.272069f;
                  }
              } else {
                  if (features[15] < 1.984251f) {
                      if (features[12] < -3.601221f) {
                          if (features[15] < 1.401808f) {
                              return -2.731000f;
                          } else {
                              return -6.590556f;
                          }
                      } else {
                          if (features[17] < 0.010384f) {
                              return -0.306168f;
                          } else {
                              return -1.484093f;
                          }
                      }
                  } else {
                      if (features[25] < 0.795752f) {
                          if (features[13] < -14.627870f) {
                              return -2.258235f;
                          } else {
                              return -3.562956f;
                          }
                      } else {
                          return -7.980588f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_30(const float *features, int32_t features_length) {
          if (features[25] < -0.095894f) {
              if (features[4] < -0.201964f) {
                  if (features[16] < 5.616600f) {
                      if (features[0] < 28.419966f) {
                          if (features[25] < -0.921536f) {
                              return -1.714706f;
                          } else {
                              return 0.414323f;
                          }
                      } else {
                          if (features[8] < 90.351783f) {
                              return 1.798800f;
                          } else {
                              return -0.306000f;
                          }
                      }
                  } else {
                      if (features[20] < -0.865132f) {
                          return 4.925988f;
                      } else {
                          if (features[0] < 32.996222f) {
                              return 1.212500f;
                          } else {
                              return 2.685818f;
                          }
                      }
                  }
              } else {
                  if (features[2] < 0.037338f) {
                      if (features[21] < 0.781180f) {
                          if (features[6] < 27.342793f) {
                              return 0.039900f;
                          } else {
                              return 0.853254f;
                          }
                      } else {
                          if (features[0] < 30.667031f) {
                              return -0.706429f;
                          } else {
                              return 0.441714f;
                          }
                      }
                  } else {
                      if (features[16] < 6.097288f) {
                          if (features[0] < 32.284750f) {
                              return 0.256173f;
                          } else {
                              return 1.050117f;
                          }
                      } else {
                          if (features[10] < -0.096567f) {
                              return 0.327187f;
                          } else {
                              return -1.134762f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 7.713055f) {
                  if (features[22] < 0.357646f) {
                      if (features[0] < 30.843448f) {
                          if (features[6] < 30.159192f) {
                              return 0.047977f;
                          } else {
                              return 0.211042f;
                          }
                      } else {
                          if (features[25] < 0.459630f) {
                              return 1.451818f;
                          } else {
                              return 0.345581f;
                          }
                      }
                  } else {
                      if (features[12] < -3.444093f) {
                          if (features[20] < 0.405852f) {
                              return -1.606667f;
                          } else {
                              return -6.053985f;
                          }
                      } else {
                          if (features[16] < 4.791888f) {
                              return -0.211884f;
                          } else {
                              return -1.288912f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 33.289986f) {
                      if (features[6] < 29.178327f) {
                          return -10.570625f;
                      } else {
                          if (features[16] < 9.167587f) {
                              return -5.176875f;
                          } else {
                              return -3.324091f;
                          }
                      }
                  } else {
                      if (features[7] < 0.448569f) {
                          return 4.297619f;
                      } else {
                          return -0.310625f;
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_31(const float *features, int32_t features_length) {
          if (features[25] < 0.029825f) {
              if (features[3] < -0.314634f) {
                  if (features[8] < 86.057551f) {
                      if (features[26] < -0.158853f) {
                          if (features[0] < 34.172951f) {
                              return 3.791360f;
                          } else {
                              return 2.070952f;
                          }
                      } else {
                          if (features[8] < 70.789295f) {
                              return 3.796667f;
                          } else {
                              return 0.773140f;
                          }
                      }
                  } else {
                      if (features[23] < 0.163725f) {
                          return -0.248333f;
                      } else {
                          if (features[5] < -0.574227f) {
                              return 0.762143f;
                          } else {
                              return 0.675333f;
                          }
                      }
                  }
              } else {
                  if (features[0] < 27.455902f) {
                      if (features[6] < 27.156968f) {
                          if (features[13] < 1.497055f) {
                              return 0.183417f;
                          } else {
                              return -0.615735f;
                          }
                      } else {
                          if (features[26] < -0.757229f) {
                              return 1.875625f;
                          } else {
                              return 0.371538f;
                          }
                      }
                  } else {
                      if (features[21] < -1.272806f) {
                          if (features[2] < -0.031780f) {
                              return 2.183624f;
                          } else {
                              return 1.106988f;
                          }
                      } else {
                          if (features[8] < 57.122671f) {
                              return 1.649630f;
                          } else {
                              return 0.540963f;
                          }
                      }
                  }
              }
          } else {
              if (features[10] < -1.043033f) {
                  if (features[6] < 29.706081f) {
                      if (features[13] < -4.535903f) {
                          if (features[7] < 0.141826f) {
                              return -5.441905f;
                          } else {
                              return -9.091250f;
                          }
                      } else {
                          if (features[16] < 4.804435f) {
                              return -0.768772f;
                          } else {
                              return -2.772273f;
                          }
                      }
                  } else {
                      if (features[1] < 0.094475f) {
                          if (features[7] < 0.262155f) {
                              return -0.260790f;
                          } else {
                              return 1.574519f;
                          }
                      } else {
                          if (features[26] < -0.898821f) {
                              return -3.010000f;
                          } else {
                              return -1.242903f;
                          }
                      }
                  }
              } else {
                  if (features[22] < 4.114755f) {
                      if (features[15] < 1.012169f) {
                          if (features[14] < 65.458449f) {
                              return 1.154146f;
                          } else {
                              return -0.056689f;
                          }
                      } else {
                          if (features[16] < 1.770231f) {
                              return 0.778421f;
                          } else {
                              return 1.044897f;
                          }
                      }
                  } else {
                      if (features[8] < 64.259498f) {
                          if (features[13] < -5.145816f) {
                              return -1.407143f;
                          } else {
                              return 0.399853f;
                          }
                      } else {
                          if (features[16] < 7.679447f) {
                              return -1.383458f;
                          } else {
                              return -4.109091f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_32(const float *features, int32_t features_length) {
          if (features[25] < -0.160745f) {
              if (features[3] < -0.347010f) {
                  if (features[8] < 84.518785f) {
                      if (features[9] < 0.912975f) {
                          if (features[19] < -0.304594f) {
                              return 3.758990f;
                          } else {
                              return 1.885000f;
                          }
                      } else {
                          return 4.166370f;
                      }
                  } else {
                      return 0.768491f;
                  }
              } else {
                  if (features[4] < -0.358609f) {
                      if (features[1] < 0.026362f) {
                          if (features[14] < 93.706885f) {
                              return 1.843824f;
                          } else {
                              return -0.052333f;
                          }
                      } else {
                          return 0.112184f;
                      }
                  } else {
                      if (features[1] < 0.136374f) {
                          if (features[8] < 90.486584f) {
                              return 0.769848f;
                          } else {
                              return 0.104583f;
                          }
                      } else {
                          if (features[25] < -0.526641f) {
                              return -0.214684f;
                          } else {
                              return -0.775357f;
                          }
                      }
                  }
              }
          } else {
              if (features[12] < -5.377321f) {
                  if (features[4] < 2.631055f) {
                      return -5.964138f;
                  } else {
                      return -2.990526f;
                  }
              } else {
                  if (features[22] < 2.571523f) {
                      if (features[9] < -0.627632f) {
                          if (features[12] < -0.160617f) {
                              return 0.572866f;
                          } else {
                              return -0.100000f;
                          }
                      } else {
                          if (features[7] < 0.137188f) {
                              return 0.096878f;
                          } else {
                              return 0.459821f;
                          }
                      }
                  } else {
                      if (features[8] < 62.220238f) {
                          if (features[12] < -0.143424f) {
                              return 1.837927f;
                          } else {
                              return -0.322963f;
                          }
                      } else {
                          if (features[11] < -1.726129f) {
                              return -2.049039f;
                          } else {
                              return -0.818211f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_33(const float *features, int32_t features_length) {
          if (features[25] < -0.389470f) {
              if (features[6] < 28.249133f) {
                  if (features[16] < 3.084248f) {
                      if (features[0] < 27.361540f) {
                          if (features[13] < 0.394486f) {
                              return 0.394704f;
                          } else {
                              return -0.268390f;
                          }
                      } else {
                          return -2.539321f;
                      }
                  } else {
                      if (features[21] < -0.226424f) {
                          if (features[21] < -0.925594f) {
                              return 2.893333f;
                          } else {
                              return 0.590000f;
                          }
                      } else {
                          if (features[6] < 26.403732f) {
                              return -1.110476f;
                          } else {
                              return 0.063968f;
                          }
                      }
                  }
              } else {
                  if (features[21] < -2.284654f) {
                      if (features[23] < 0.904083f) {
                          return 3.398235f;
                      } else {
                          return 1.458611f;
                      }
                  } else {
                      if (features[22] < 6.446763f) {
                          if (features[26] < -0.800776f) {
                              return 2.085185f;
                          } else {
                              return 0.644367f;
                          }
                      } else {
                          if (features[4] < 0.401431f) {
                              return 2.901835f;
                          } else {
                              return -0.603333f;
                          }
                      }
                  }
              }
          } else {
              if (features[26] < -0.008345f) {
                  if (features[26] < -0.697834f) {
                      if (features[14] < 56.445417f) {
                          return 2.537385f;
                      } else {
                          if (features[25] < 0.175869f) {
                              return 0.397614f;
                          } else {
                              return -0.861172f;
                          }
                      }
                  } else {
                      if (features[12] < -3.158785f) {
                          if (features[8] < 69.407518f) {
                              return -3.318333f;
                          } else {
                              return -7.694750f;
                          }
                      } else {
                          if (features[16] < 4.962640f) {
                              return -0.630534f;
                          } else {
                              return -3.378246f;
                          }
                      }
                  }
              } else {
                  if (features[9] < 1.259210f) {
                      if (features[22] < 2.576545f) {
                          if (features[12] < -1.438664f) {
                              return 0.824651f;
                          } else {
                              return 0.179175f;
                          }
                      } else {
                          if (features[10] < -1.016986f) {
                              return 0.685833f;
                          } else {
                              return -0.400171f;
                          }
                      }
                  } else {
                      return -1.056875f;
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_34(const float *features, int32_t features_length) {
          if (features[25] < 0.124718f) {
              if (features[21] < -1.664751f) {
                  if (features[16] < 4.521165f) {
                      if (features[5] < 0.003507f) {
                          if (features[14] < 91.398580f) {
                              return 2.166212f;
                          } else {
                              return 0.394667f;
                          }
                      } else {
                          return -0.798947f;
                      }
                  } else {
                      if (features[19] < -0.478911f) {
                          return 4.051481f;
                      } else {
                          return 3.798667f;
                      }
                  }
              } else {
                  if (features[7] < 0.164605f) {
                      if (features[20] < 0.741000f) {
                          if (features[21] < 0.102162f) {
                              return 0.593526f;
                          } else {
                              return 0.039540f;
                          }
                      } else {
                          if (features[1] < 0.031563f) {
                              return 0.137460f;
                          } else {
                              return -0.827222f;
                          }
                      }
                  } else {
                      if (features[14] < 80.625306f) {
                          if (features[4] < 0.408082f) {
                              return 2.175011f;
                          } else {
                              return 0.356468f;
                          }
                      } else {
                          if (features[26] < -0.927198f) {
                              return 1.345000f;
                          } else {
                              return -0.099684f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 5.658861f) {
                  if (features[22] < 2.386567f) {
                      if (features[6] < 27.384508f) {
                          if (features[6] < 25.917589f) {
                              return -0.590556f;
                          } else {
                              return 0.065700f;
                          }
                      } else {
                          if (features[21] < -2.639642f) {
                              return 2.049474f;
                          } else {
                              return 0.071255f;
                          }
                      }
                  } else {
                      if (features[16] < 4.311139f) {
                          if (features[11] < 1.180004f) {
                              return -0.230334f;
                          } else {
                              return 0.791173f;
                          }
                      } else {
                          if (features[2] < -0.009143f) {
                              return -0.618907f;
                          } else {
                              return -1.808988f;
                          }
                      }
                  }
              } else {
                  if (features[14] < 76.962478f) {
                      if (features[5] < 3.769040f) {
                          if (features[6] < 33.693314f) {
                              return -1.118407f;
                          } else {
                              return 1.619420f;
                          }
                      } else {
                          return -3.735000f;
                      }
                  } else {
                      if (features[0] < 29.488213f) {
                          if (features[23] < 0.347191f) {
                              return -8.945625f;
                          } else {
                              return -5.987000f;
                          }
                      } else {
                          if (features[12] < -4.581878f) {
                              return -5.722667f;
                          } else {
                              return -2.077706f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_35(const float *features, int32_t features_length) {
          if (features[13] < -10.524352f) {
              if (features[25] < 0.802291f) {
                  if (features[16] < 6.702407f) {
                      return -2.115625f;
                  } else {
                      return -0.982778f;
                  }
              } else {
                  return -8.891111f;
              }
          } else {
              if (features[1] < -0.007872f) {
                  if (features[13] < 2.346395f) {
                      if (features[0] < 33.244302f) {
                          if (features[25] < 0.104328f) {
                              return 0.772593f;
                          } else {
                              return -0.063869f;
                          }
                      } else {
                          if (features[16] < 8.206605f) {
                              return 0.378000f;
                          } else {
                              return 3.347419f;
                          }
                      }
                  } else {
                      if (features[22] < 4.290685f) {
                          if (features[0] < 28.820127f) {
                              return 0.162685f;
                          } else {
                              return 0.886249f;
                          }
                      } else {
                          if (features[2] < -0.174720f) {
                              return 4.234939f;
                          } else {
                              return 1.199355f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.389984f) {
                      if (features[6] < 35.570371f) {
                          if (features[22] < 5.058464f) {
                              return 0.355390f;
                          } else {
                              return 1.457222f;
                          }
                      } else {
                          return 1.348421f;
                      }
                  } else {
                      if (features[16] < 4.792950f) {
                          if (features[4] < 0.176888f) {
                              return 0.009784f;
                          } else {
                              return -0.731517f;
                          }
                      } else {
                          if (features[14] < 81.483257f) {
                              return -0.914448f;
                          } else {
                              return -3.878600f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_36(const float *features, int32_t features_length) {
          if (features[25] < -0.644422f) {
              if (features[16] < 5.060331f) {
                  if (features[0] < 28.498126f) {
                      if (features[0] < 26.630889f) {
                          return -1.100000f;
                      } else {
                          if (features[25] < -0.904389f) {
                              return -0.355503f;
                          } else {
                              return 0.296967f;
                          }
                      }
                  } else {
                      if (features[16] < 0.166142f) {
                          if (features[25] < -0.767979f) {
                              return 2.123953f;
                          } else {
                              return 0.434000f;
                          }
                      } else {
                          if (features[6] < 29.657492f) {
                              return -0.108275f;
                          } else {
                              return 0.888007f;
                          }
                      }
                  }
              } else {
                  if (features[20] < -0.446136f) {
                      if (features[14] < 73.684277f) {
                          return 3.045673f;
                      } else {
                          return 4.720000f;
                      }
                  } else {
                      if (features[16] < 7.570682f) {
                          return 1.728889f;
                      } else {
                          if (features[26] < -0.525413f) {
                              return 4.223182f;
                          } else {
                              return 1.994118f;
                          }
                      }
                  }
              }
          } else {
              if (features[16] < 5.378115f) {
                  if (features[25] < 0.591749f) {
                      if (features[26] < 0.276355f) {
                          if (features[10] < 2.919875f) {
                              return 0.825271f;
                          } else {
                              return 5.689306f;
                          }
                      } else {
                          if (features[22] < 0.568997f) {
                              return 0.319197f;
                          } else {
                              return -0.013288f;
                          }
                      }
                  } else {
                      if (features[22] < 2.315695f) {
                          if (features[0] < 28.349733f) {
                              return -0.046351f;
                          } else {
                              return 0.262105f;
                          }
                      } else {
                          if (features[16] < 4.153163f) {
                              return -0.277728f;
                          } else {
                              return -1.711107f;
                          }
                      }
                  }
              } else {
                  if (features[26] < -0.380869f) {
                      if (features[6] < 33.886415f) {
                          if (features[26] < -0.872760f) {
                              return -0.826448f;
                          } else {
                              return -2.723230f;
                          }
                      } else {
                          if (features[22] < 8.696720f) {
                              return -0.346429f;
                          } else {
                              return 2.207262f;
                          }
                      }
                  } else {
                      return -7.927619f;
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_37(const float *features, int32_t features_length) {
          if (features[25] < 0.795481f) {
              if (features[18] < 0.069005f) {
                  if (features[22] < 6.961412f) {
                      if (features[1] < -0.020377f) {
                          if (features[6] < 26.848804f) {
                              return -0.335309f;
                          } else {
                              return 0.853791f;
                          }
                      } else {
                          if (features[21] < -2.418029f) {
                              return 1.940244f;
                          } else {
                              return 0.042470f;
                          }
                      }
                  } else {
                      if (features[7] < 0.507263f) {
                          if (features[19] < -0.264754f) {
                              return 4.119563f;
                          } else {
                              return 2.404310f;
                          }
                      } else {
                          if (features[14] < 73.711316f) {
                              return 0.732386f;
                          } else {
                              return -3.070667f;
                          }
                      }
                  }
              } else {
                  if (features[25] < -0.572040f) {
                      if (features[0] < 27.129842f) {
                          if (features[9] < 0.117977f) {
                              return -0.171429f;
                          } else {
                              return -0.841176f;
                          }
                      } else {
                          if (features[0] < 31.627559f) {
                              return 0.575875f;
                          } else {
                              return 1.335000f;
                          }
                      }
                  } else {
                      if (features[25] < -0.215170f) {
                          if (features[16] < 5.010141f) {
                              return 0.841330f;
                          } else {
                              return -0.395263f;
                          }
                      } else {
                          if (features[16] < 3.946006f) {
                              return 0.594303f;
                          } else {
                              return -1.472939f;
                          }
                      }
                  }
              }
          } else {
              if (features[13] < -11.799040f) {
                  return -8.811765f;
              } else {
                  if (features[16] < 4.552367f) {
                      if (features[26] < -0.099374f) {
                          if (features[3] < 0.054207f) {
                              return -0.278339f;
                          } else {
                              return -0.948315f;
                          }
                      } else {
                          if (features[14] < 67.342557f) {
                              return 0.696346f;
                          } else {
                              return 0.114858f;
                          }
                      }
                  } else {
                      if (features[10] < -0.309898f) {
                          if (features[12] < -2.256450f) {
                              return -4.704138f;
                          } else {
                              return -2.878095f;
                          }
                      } else {
                          if (features[25] < 0.871407f) {
                              return -4.017222f;
                          } else {
                              return -0.901000f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_38(const float *features, int32_t features_length) {
          if (features[25] < -0.674138f) {
              if (features[22] < 4.358537f) {
                  if (features[26] < -0.194920f) {
                      if (features[26] < -0.332508f) {
                          if (features[14] < 90.168852f) {
                              return 0.556573f;
                          } else {
                              return -0.462000f;
                          }
                      } else {
                          if (features[15] < 1.191250f) {
                              return -0.492270f;
                          } else {
                              return -2.164967f;
                          }
                      }
                  } else {
                      if (features[0] < 28.774641f) {
                          if (features[26] < 0.188140f) {
                              return -0.515485f;
                          } else {
                              return 0.296486f;
                          }
                      } else {
                          if (features[6] < 30.687607f) {
                              return 0.720320f;
                          } else {
                              return 1.476042f;
                          }
                      }
                  }
              } else {
                  if (features[5] < -1.614823f) {
                      return 4.511401f;
                  } else {
                      if (features[16] < 4.506415f) {
                          return -0.032000f;
                      } else {
                          if (features[2] < -0.026146f) {
                              return 2.999037f;
                          } else {
                              return 1.191667f;
                          }
                      }
                  }
              }
          } else {
              if (features[25] < 0.448495f) {
                  if (features[19] < 0.680027f) {
                      if (features[25] < 0.336241f) {
                          if (features[22] < 5.335194f) {
                              return 0.634040f;
                          } else {
                              return -0.067651f;
                          }
                      } else {
                          if (features[6] < 32.114807f) {
                              return -0.133603f;
                          } else {
                              return -2.691875f;
                          }
                      }
                  } else {
                      if (features[2] < 0.184108f) {
                          if (features[16] < 5.725194f) {
                              return -0.284615f;
                          } else {
                              return -1.183000f;
                          }
                      } else {
                          return -2.604444f;
                      }
                  }
              } else {
                  if (features[16] < 4.325304f) {
                      if (features[21] < -0.529799f) {
                          return 2.158815f;
                      } else {
                          if (features[16] < 3.367512f) {
                              return 0.100739f;
                          } else {
                              return -0.511967f;
                          }
                      }
                  } else {
                      if (features[25] < 0.723411f) {
                          if (features[0] < 32.056442f) {
                              return -1.361261f;
                          } else {
                              return 1.070817f;
                          }
                      } else {
                          if (features[12] < -2.878832f) {
                              return -6.398947f;
                          } else {
                              return -2.152873f;
                          }
                      }
                  }
              }
          }
        }
        

static inline float forecast_humidity_tree_39(const float *features, int32_t features_length) {
          if (features[5] < 0.058352f) {
              if (features[4] < -0.493203f) {
                  if (features[8] < 73.139304f) {
                      if (features[14] < 68.145949f) {
                          if (features[16] < 6.560259f) {
                              return 4.608421f;
                          } else {
                              return 2.940658f;
                          }
                      } else {
                          if (features[25] < -0.350283f) {
                              return 2.958750f;
                          } else {
                              return -0.253852f;
                          }
                      }
                  } else {
                      if (features[14] < 78.634993f) {
                          if (features[1] < -0.156276f) {
                              return 2.898889f;
                          } else {
                              return 1.223438f;
                          }
                      } else {
                          if (features[0] < 26.990468f) {
                              return -0.410667f;
                          } else {
                              return 0.907843f;
                          }
                      }
                  }
              } else {
                  if (features[4] < -0.360155f) {
                      if (features[14] < 86.633078f) {
                          if (features[1] < -0.039926f) {
                              return 2.165636f;
                          } else {
                              return 0.566452f;
                          }
                      } else {
                          if (features[18] < 0.008626f) {
                              return 0.361212f;
                          } else {
                              return -0.576667f;
                          }
                      }
                  } else {
                      if (features[21] < -2.685272f) {
                          return 2.341563f;
                      } else {
                          if (features[21] < 0.438974f) {
                              return 0.280218f;
                          } else {
                              return -0.237109f;
                          }
                      }
                  }
              }
          } else {
              if (features[12] < -4.962437f) {
                  if (features[0] < 30.626250f) {
                      return -7.346068f;
                  } else {
                      if (features[25] < 0.529170f) {
                          return 1.420667f;
                      } else {
                          return -3.518889f;
                      }
                  }
              } else {
                  if (features[6] < 32.268550f) {
                      if (features[16] < 6.743713f) {
                          if (features[25] < 0.313229f) {
                              return 0.249179f;
                          } else {
                              return -0.992129f;
                          }
                      } else {
                          return -5.037083f;
                      }
                  } else {
                      if (features[16] < 8.935844f) {
                          if (features[14] < 68.461889f) {
                              return -0.040648f;
                          } else {
                              return 3.817500f;
                          }
                      } else {
                          return 2.500867f;
                      }
                  }
              }
          }
        }
        

static const uint8_t forecast_humidity_leaves[4084] = { 0, 0, 160, 62, 89, 9, 37, 64, 83, 20, 183, 63, 161, 214, 1, 63, 249, 45, 219, 189, 70, 182, 131, 63, 148, 62, 169, 62, 187, 219, 216, 190, 150, 7, 146, 64, 54, 234, 43, 64, 172, 226, 119, 64, 161, 109, 154, 63, 176, 5, 155, 62, 171, 124, 23, 63, 154, 82, 49, 191, 106, 77, 139, 192, 154, 212, 12, 190, 11, 235, 191, 192, 93, 93, 141, 192, 107, 29, 161, 63, 190, 124, 153, 62, 183, 176, 236, 189, 152, 193, 174, 191, 171, 133, 156, 61, 68, 209, 170, 191, 101, 87, 7, 191, 30, 168, 157, 191, 72, 225, 2, 63, 205, 204, 44, 191, 190, 121, 149, 191, 175, 44, 112, 62, 21, 58, 57, 64, 62, 121, 88, 63, 133, 235, 129, 191, 92, 143, 74, 64, 96, 249, 82, 191, 109, 16, 127, 63, 244, 89, 185, 63, 200, 254, 19, 63, 2, 85, 124, 190, 25, 42, 187, 62, 158, 18, 228, 63, 170, 254, 60, 63, 188, 1, 106, 61, 132, 87, 43, 190, 231, 65, 25, 62, 33, 51, 12, 191, 27, 250, 193, 191, 159, 171, 173, 191, 112, 94, 77, 190, 14, 175, 29, 193, 145, 15, 178, 192, 46, 134, 52, 191, 17, 161, 116, 64, 218, 241, 13, 192, 62, 229, 142, 191, 107, 41, 61, 62, 222, 28, 87, 191, 38, 191, 48, 64, 143, 79, 207, 62, 106, 188, 52, 64, 113, 61, 202, 63, 159, 138, 101, 61, 214, 27, 135, 63, 67, 117, 52, 61, 90, 73, 160, 64, 77, 40, 46, 64, 58, 109, 168, 63, 43, 26, 42, 64, 42, 104, 49, 62, 144, 1, 125, 63, 62, 37, 11, 64, 173, 213, 209, 63, 33, 39, 79, 190, 197, 241, 57, 191, 16, 248, 133, 61, 165, 79, 107, 62, 27, 66, 239, 190, 234, 234, 216, 191, 15, 63, 164, 191, 91, 169, 181, 192, 124, 243, 154, 190, 77, 97, 251, 192, 10, 104, 2, 192, 218, 179, 220, 192, 141, 151, 144, 192, 234, 114, 123, 62, 71, 2, 162, 63, 123, 133, 7, 61, 87, 250, 135, 61, 25, 165, 157, 63, 183, 63, 88, 64, 88, 19, 155, 64, 229, 157, 30, 64, 227, 165, 75, 63, 42, 13, 37, 192, 135, 212, 84, 62, 185, 235, 142, 64, 156, 118, 31, 189, 21, 163, 149, 63, 158, 30, 38, 190, 148, 153, 177, 191, 244, 39, 7, 64, 227, 117, 132, 64, 160, 211, 70, 61, 155, 183, 143, 63, 2, 30, 164, 63, 70, 32, 239, 62, 12, 170, 188, 189, 80, 7, 117, 191, 21, 86, 248, 62, 215, 164, 67, 190, 230, 137, 122, 190, 155, 28, 144, 191, 180, 179, 147, 191, 113, 61, 28, 192, 216, 176, 160, 191, 87, 49, 89, 192, 172, 125, 3, 193, 133, 133, 18, 64, 22, 187, 231, 190, 64, 251, 203, 191, 92, 143, 66, 62, 8, 110, 20, 63, 71, 88, 238, 190, 4, 76, 253, 61, 46, 0, 15, 63, 143, 194, 37, 191, 15, 80, 22, 192, 78, 231, 0, 191, 19, 92, 244, 62, 56, 172, 100, 64, 0, 207, 89, 63, 153, 243, 129, 63, 165, 33, 71, 64, 212, 201, 59, 190, 123, 49, 34, 63, 13, 42, 56, 62, 222, 237, 44, 191, 140, 192, 136, 189, 130, 29, 148, 191, 252, 94, 167, 61, 62, 48, 188, 190, 81, 114, 166, 61, 189, 39, 177, 62, 12, 189, 128, 191, 223, 188, 26, 62, 14, 17, 14, 191, 185, 219, 31, 192, 100, 168, 236, 192, 208, 109, 58, 64, 23, 255, 229, 191, 115, 251, 195, 192, 151, 55, 2, 193, 123, 20, 110, 192, 42, 21, 39, 192, 135, 156, 139, 63, 151, 206, 80, 63, 23, 9, 155, 188, 7, 114, 165, 63, 92, 102, 132, 190, 115, 100, 132, 63, 39, 18, 36, 64, 61, 161, 80, 63, 169, 50, 70, 191, 2, 81, 85, 60, 197, 22, 154, 62, 234, 185, 35, 63, 243, 124, 217, 190, 212, 8, 11, 192, 141, 114, 124, 191, 162, 69, 194, 192, 156, 53, 47, 192, 245, 73, 34, 192, 4, 227, 11, 61, 186, 231, 127, 63, 75, 43, 251, 189, 61, 75, 182, 63, 7, 58, 141, 190, 59, 191, 4, 63, 190, 78, 61, 64, 198, 195, 157, 64, 100, 179, 97, 64, 31, 133, 63, 63, 246, 40, 52, 64, 139, 185, 154, 63, 149, 254, 129, 62, 6, 129, 149, 62, 183, 237, 29, 64, 165, 0, 136, 62, 134, 129, 119, 191, 37, 159, 241, 191, 54, 23, 18, 64, 28, 94, 251, 191, 254, 75, 7, 193, 7, 249, 24, 191, 171, 166, 89, 63, 227, 165, 27, 191, 125, 109, 233, 191, 73, 189, 142, 190, 138, 63, 34, 64, 163, 125, 41, 63, 22, 56, 154, 191, 130, 31, 248, 190, 253, 105, 53, 62, 215, 152, 168, 63, 124, 101, 165, 62, 245, 51, 19, 64, 2, 43, 183, 64, 43, 9, 103, 64, 72, 115, 5, 64, 143, 19, 192, 62, 146, 118, 44, 60, 31, 236, 40, 64, 180, 35, 26, 63, 178, 144, 133, 189, 178, 206, 85, 63, 137, 231, 58, 189, 4, 19, 243, 192, 122, 53, 57, 192, 119, 190, 159, 189, 220, 61, 44, 64, 242, 98, 168, 191, 168, 94, 66, 192, 32, 106, 90, 191, 23, 217, 206, 63, 155, 0, 72, 191, 156, 205, 164, 190, 166, 56, 252, 62, 62, 140, 219, 63, 201, 60, 41, 190, 253, 144, 171, 63, 118, 118, 118, 62, 216, 249, 155, 64, 3, 125, 83, 64, 123, 20, 215, 63, 72, 225, 116, 192, 244, 14, 156, 191, 182, 217, 137, 63, 67, 101, 63, 192, 182, 58, 46, 193, 64, 30, 148, 192, 228, 15, 35, 192, 199, 221, 46, 64, 101, 245, 79, 62, 46, 192, 11, 62, 86, 130, 17, 191, 108, 236, 207, 191, 29, 212, 107, 191, 130, 32, 136, 63, 234, 69, 82, 63, 132, 142, 158, 191, 186, 52, 82, 192, 38, 214, 129, 191, 184, 16, 24, 193, 200, 189, 62, 192, 152, 80, 8, 64, 19, 3, 67, 63, 24, 209, 22, 64, 161, 23, 138, 63, 58, 109, 8, 64, 236, 241, 187, 62, 23, 42, 154, 190, 34, 52, 248, 62, 39, 246, 130, 61, 233, 147, 176, 63, 104, 69, 235, 62, 179, 86, 7, 63, 164, 51, 49, 191, 138, 214, 5, 191, 199, 12, 6, 192, 135, 209, 46, 64, 43, 155, 184, 63, 94, 81, 34, 191, 76, 169, 138, 190, 151, 120, 216, 62, 156, 222, 244, 190, 9, 83, 230, 63, 133, 244, 79, 63, 47, 186, 144, 64, 128, 116, 93, 64, 69, 59, 169, 63, 248, 12, 123, 64, 114, 139, 15, 192, 195, 218, 23, 63, 4, 231, 140, 192, 94, 1, 22, 193, 57, 71, 252, 62, 106, 137, 192, 191, 148, 138, 192, 187, 240, 159, 10, 64, 177, 191, 12, 63, 29, 137, 15, 191, 87, 149, 43, 191, 46, 187, 231, 191, 217, 100, 19, 62, 27, 243, 85, 191, 111, 18, 65, 64, 242, 121, 254, 62, 153, 75, 148, 61, 28, 170, 2, 191, 123, 20, 184, 192, 229, 208, 98, 192, 35, 148, 41, 64, 196, 217, 94, 63, 36, 77, 9, 64, 109, 171, 132, 64, 30, 236, 82, 64, 240, 167, 70, 63, 230, 242, 132, 62, 73, 206, 98, 191, 9, 237, 53, 190, 47, 92, 96, 63, 17, 111, 215, 62, 54, 76, 18, 191, 254, 19, 17, 64, 69, 59, 73, 192, 77, 147, 102, 191, 190, 141, 203, 192, 7, 135, 146, 191, 85, 127, 30, 62, 133, 107, 188, 192, 212, 15, 192, 191, 233, 100, 49, 63, 130, 183, 70, 189, 61, 10, 87, 64, 202, 106, 25, 63, 208, 208, 203, 62, 245, 229, 178, 190, 113, 61, 206, 191, 115, 115, 51, 192, 29, 236, 252, 63, 123, 187, 109, 63, 114, 117, 199, 62, 138, 19, 173, 191, 132, 50, 66, 64, 219, 94, 32, 64, 0, 42, 220, 63, 116, 218, 192, 190, 154, 153, 207, 63, 229, 72, 180, 62, 113, 246, 97, 190, 92, 143, 210, 190, 146, 165, 161, 61, 143, 2, 27, 193, 72, 225, 110, 192, 78, 253, 30, 191, 228, 83, 187, 189, 102, 103, 161, 191, 46, 54, 56, 190, 49, 91, 138, 192, 212, 236, 156, 63, 244, 226, 219, 62, 35, 9, 93, 187, 235, 188, 196, 191, 4, 94, 48, 63, 106, 1, 252, 63, 100, 130, 3, 64, 95, 29, 101, 64, 100, 97, 11, 191, 226, 147, 53, 61, 73, 206, 229, 62, 185, 89, 200, 191, 249, 94, 122, 62, 16, 201, 221, 190, 82, 249, 176, 62, 128, 194, 214, 191, 189, 255, 206, 62, 33, 11, 172, 61, 224, 252, 133, 189, 109, 101, 240, 190, 77, 138, 248, 190, 82, 54, 84, 62, 160, 26, 67, 191, 68, 211, 24, 192, 72, 225, 4, 192, 55, 208, 149, 192, 234, 74, 79, 192, 179, 12, 17, 192, 184, 30, 253, 192, 11, 165, 214, 192, 136, 87, 98, 64, 127, 106, 196, 63, 69, 81, 36, 63, 199, 250, 208, 61, 31, 133, 107, 191, 236, 120, 54, 63, 185, 41, 112, 187, 184, 158, 138, 64, 239, 187, 8, 63, 60, 152, 67, 64, 43, 26, 41, 191, 176, 80, 135, 62, 235, 128, 11, 64, 67, 147, 74, 64, 55, 208, 233, 191, 156, 185, 251, 61, 32, 14, 114, 191, 223, 194, 108, 62, 51, 2, 32, 191, 249, 75, 142, 191, 131, 109, 48, 192, 224, 32, 29, 193, 49, 8, 172, 189, 133, 235, 76, 192, 195, 245, 112, 64, 14, 16, 33, 64, 170, 118, 67, 63, 116, 218, 192, 63, 204, 183, 23, 63, 151, 32, 59, 190, 167, 111, 117, 63, 202, 106, 249, 189, 10, 215, 79, 64, 49, 26, 23, 64, 244, 66, 58, 63, 50, 130, 9, 63, 162, 69, 118, 63, 236, 81, 56, 191, 154, 246, 5, 189, 150, 84, 54, 62, 13, 255, 66, 190, 158, 42, 129, 64, 51, 62, 188, 190, 230, 185, 211, 191, 212, 6, 38, 192, 111, 25, 237, 192, 227, 216, 141, 63, 221, 8, 65, 64, 157, 54, 168, 64, 39, 136, 17, 64, 10, 215, 3, 64, 68, 84, 41, 63, 197, 95, 83, 61, 145, 175, 130, 63, 199, 8, 166, 62, 0, 0, 40, 64, 236, 81, 248, 190, 82, 184, 222, 62, 116, 167, 26, 191, 101, 26, 156, 61, 78, 49, 106, 190, 174, 71, 97, 61, 1, 185, 55, 64, 133, 235, 81, 62, 67, 252, 80, 63, 34, 34, 34, 191, 5, 33, 101, 61, 151, 84, 142, 63, 237, 195, 103, 191, 71, 178, 72, 61, 244, 137, 124, 191, 52, 28, 160, 191, 110, 248, 202, 191, 62, 132, 176, 192, 226, 25, 30, 62, 22, 178, 176, 191, 94, 186, 177, 192, 109, 160, 138, 192, 70, 182, 225, 192, 221, 107, 174, 190, 48, 252, 97, 62, 63, 104, 137, 63, 23, 242, 23, 64, 112, 98, 42, 64, 47, 183, 63, 62, 204, 86, 176, 62, 217, 30, 20, 191, 12, 111, 56, 63, 131, 219, 210, 60, 189, 154, 184, 63, 104, 210, 168, 191, 81, 255, 134, 62, 198, 209, 217, 190, 26, 182, 183, 191, 95, 160, 55, 192, 85, 94, 208, 63, 115, 112, 165, 62, 27, 222, 1, 64, 118, 250, 138, 64, 66, 195, 227, 190, 26, 7, 158, 62, 226, 232, 134, 63, 218, 234, 16, 63, 93, 110, 135, 64, 102, 102, 2, 64, 117, 147, 120, 64, 218, 64, 167, 189, 119, 119, 183, 191, 89, 219, 30, 64, 27, 29, 229, 62, 4, 136, 150, 61, 171, 247, 39, 191, 205, 204, 136, 191, 31, 82, 171, 190, 238, 239, 238, 191, 133, 235, 173, 192, 202, 170, 103, 64, 2, 43, 39, 192, 167, 123, 64, 191, 207, 17, 12, 193, 82, 56, 168, 192, 123, 20, 110, 191, 141, 224, 106, 64, 76, 91, 80, 64, 209, 143, 104, 63, 37, 6, 17, 64, 125, 63, 53, 63, 165, 79, 154, 62, 190, 67, 98, 191, 119, 4, 197, 190, 205, 9, 11, 63, 19, 222, 183, 189, 84, 89, 47, 63, 220, 3, 35, 63, 195, 31, 131, 190, 240, 144, 143, 63, 0, 45, 134, 189, 194, 111, 37, 62, 154, 44, 212, 63, 175, 227, 159, 61, 147, 211, 104, 190, 192, 147, 213, 190, 136, 125, 160, 191, 143, 26, 235, 188, 127, 14, 5, 63, 70, 176, 149, 64, 64, 159, 144, 191, 1, 229, 118, 192, 238, 4, 209, 192, 216, 94, 56, 192, 66, 25, 197, 192, 225, 211, 212, 190, 173, 150, 35, 192, 82, 184, 54, 193, 72, 225, 254, 191, 70, 48, 157, 192, 138, 159, 14, 64, 205, 204, 228, 191, 76, 46, 68, 64, 95, 137, 78, 63, 56, 125, 252, 62, 164, 112, 29, 191, 113, 61, 150, 64, 78, 226, 0, 64, 85, 156, 109, 64, 154, 153, 25, 64, 133, 247, 12, 63, 107, 83, 137, 190, 156, 31, 135, 191, 24, 171, 45, 62, 192, 244, 240, 189, 149, 165, 21, 191, 63, 7, 176, 191, 62, 111, 115, 63, 140, 250, 38, 189, 46, 216, 194, 63, 221, 188, 245, 62, 167, 126, 47, 64, 223, 168, 111, 63, 174, 199, 246, 64, 203, 14, 19, 64, 125, 63, 211, 191, 245, 115, 123, 64, 211, 100, 151, 61, 82, 53, 242, 63, 236, 81, 214, 191, 135, 68, 133, 192, 35, 219, 249, 192, 201, 73, 157, 191, 36, 60, 2, 190, 61, 32, 137, 191, 86, 14, 125, 192, 250, 95, 27, 64, 159, 218, 99, 61, 155, 230, 93, 191, 189, 26, 153, 61, 119, 249, 17, 63, 50, 34, 190, 190, 221, 36, 134, 191, 175, 166, 60, 192, 77, 71, 236, 63, 119, 49, 138, 64, 32, 57, 164, 63, 45, 55, 213, 62, 40, 27, 150, 63, 29, 160, 169, 61, 55, 23, 111, 191, 153, 174, 97, 64, 208, 46, 36, 64, 110, 216, 71, 190, 200, 185, 127, 63, 145, 237, 60, 63, 113, 61, 176, 191, 143, 92, 165, 191, 38, 150, 26, 192, 254, 65, 14, 193, 65, 167, 125, 192, 174, 71, 38, 192, 133, 235, 70, 192, 86, 104, 243, 62, 242, 103, 254, 190, 176, 202, 15, 191, 50, 78, 64, 192, 183, 63, 72, 63, 46, 148, 250, 188, 187, 0, 245, 61, 16, 50, 20, 191, 67, 101, 125, 64, 70, 168, 168, 63, 47, 120, 43, 64, 95, 43, 56, 63, 81, 245, 7, 64, 208, 174, 3, 63, 118, 139, 234, 190, 150, 67, 139, 189, 184, 30, 137, 64, 79, 174, 174, 62, 132, 228, 136, 191, 123, 20, 238, 61, 223, 188, 90, 191, 37, 242, 62, 190, 159, 165, 244, 63, 62, 232, 25, 63, 49, 8, 172, 63, 225, 122, 140, 62, 229, 13, 78, 189, 165, 153, 144, 63, 252, 107, 128, 192, 120, 171, 35, 191, 229, 23, 219, 192, 195, 245, 45, 192, 236, 209, 94, 192, 117, 13, 134, 191, 17, 17, 209, 63, 22, 130, 142, 64, 243, 174, 26, 64, 125, 32, 167, 190, 100, 118, 209, 63, 8, 118, 79, 64, 213, 5, 22, 63, 137, 127, 45, 191, 40, 2, 52, 62, 175, 252, 97, 63, 164, 217, 179, 190, 235, 216, 187, 63, 7, 23, 13, 63, 48, 150, 36, 63, 224, 201, 170, 191, 202, 84, 65, 63, 75, 2, 84, 189, 109, 160, 179, 190, 10, 104, 34, 191, 242, 216, 75, 61, 76, 55, 9, 64, 245, 130, 9, 63, 37, 181, 172, 61, 232, 109, 144, 192, 182, 8, 152, 191, 54, 171, 73, 191, 244, 247, 158, 192, 51, 51, 58, 192, 110, 151, 88, 190, 205, 241, 42, 64, 149, 29, 38, 61, 224, 148, 71, 63, 247, 136, 104, 64, 59, 223, 15, 63, 38, 180, 119, 64, 112, 192, 217, 63, 232, 173, 27, 63, 196, 238, 203, 190, 13, 163, 125, 62, 217, 252, 195, 63, 27, 26, 229, 190, 34, 23, 67, 62, 38, 169, 86, 63, 246, 143, 25, 64, 27, 194, 207, 189, 148, 147, 147, 63, 34, 156, 193, 61, 162, 178, 129, 63, 180, 222, 181, 190, 175, 44, 240, 61, 55, 75, 39, 188, 37, 149, 150, 191, 115, 7, 152, 192, 6, 38, 199, 191, 135, 215, 38, 64, 12, 2, 115, 192, 224, 149, 17, 193, 96, 219, 147, 191, 30, 166, 1, 64, 215, 163, 60, 191, 142, 147, 215, 190, 162, 178, 51, 192, 143, 2, 88, 193, 190, 121, 21, 192, 31, 133, 134, 192, 120, 62, 183, 63, 122, 129, 90, 64, 62, 18, 129, 63, 228, 131, 167, 190, 136, 222, 69, 191, 101, 105, 155, 61, 2, 149, 50, 63, 41, 92, 31, 62, 168, 233, 7, 64, 190, 171, 3, 63, 87, 98, 97, 61, 248, 152, 62, 191, 115, 178, 199, 62, 20, 241, 80, 64, 51, 121, 88, 191, 67, 166, 66, 192, 53, 188, 56, 63, 53, 104, 123, 191, 53, 191, 28, 64, 21, 76, 5, 63, 97, 134, 163, 190, 253, 73, 177, 62, 154, 153, 139, 63, 175, 37, 100, 62, 66, 8, 81, 64, 50, 46, 161, 64, 193, 68, 31, 64, 84, 227, 1, 64, 198, 176, 40, 191, 49, 164, 211, 63, 164, 215, 37, 64, 75, 23, 219, 62, 101, 135, 72, 62, 212, 66, 140, 191, 184, 181, 78, 63, 201, 216, 238, 61, 163, 123, 206, 190, 23, 93, 244, 63, 192, 252, 203, 62, 38, 89, 140, 191, 35, 102, 243, 190, 74, 58, 230, 191, 253, 221, 109, 64, 193, 192, 192, 190, 117, 102, 185, 191, 144, 16, 162, 192, 184, 30, 5, 190, 120, 228, 19, 63, 185, 41, 3, 64, 157, 54, 136, 63, 196, 184, 175, 64, 13, 130, 52, 64, 139, 223, 87, 63, 199, 84, 188, 62, 177, 14, 97, 62, 103, 89, 55, 191, 246, 40, 36, 64, 4, 106, 240, 63, 194, 60, 81, 191, 41, 254, 40, 64, 40, 211, 162, 63, 110, 167, 190, 189, 151, 252, 103, 62, 14, 166, 200, 190, 181, 211, 221, 61, 180, 200, 46, 192, 213, 229, 210, 192, 21, 194, 156, 190, 192, 246, 189, 191, 251, 96, 255, 192, 237, 134, 16, 192, 122, 7, 100, 192, 123, 123, 219, 191, 34, 34, 212, 62, 23, 63, 230, 63, 8, 172, 156, 190, 177, 161, 157, 64, 51, 51, 155, 63, 114, 228, 43, 64, 20, 110, 35, 61, 215, 110, 90, 63, 129, 216, 52, 191, 96, 40, 226, 62, 22, 41, 131, 62, 64, 106, 134, 63, 31, 133, 167, 62, 225, 63, 145, 191, 43, 131, 68, 61, 79, 27, 88, 62, 46, 213, 185, 63, 11, 240, 176, 62, 65, 167, 205, 191, 63, 186, 193, 192, 8, 248, 88, 190, 18, 251, 164, 191, 72, 33, 41, 193, 246, 168, 165, 192, 232, 189, 84, 192, 24, 134, 137, 64, 61, 10, 159, 190, 165, 165, 114, 64, 124, 138, 4, 64, 150, 252, 114, 64, 130, 236, 69, 63, 24, 75, 126, 190, 203, 27, 67, 63, 165, 226, 44, 63, 176, 209, 59, 62, 212, 160, 29, 191, 123, 20, 240, 63, 74, 58, 190, 62, 128, 192, 11, 64, 197, 177, 141, 63, 16, 39, 211, 63, 141, 124, 10, 63, 21, 36, 174, 192, 195, 117, 17, 193, 61, 206, 68, 191, 235, 108, 49, 192, 76, 134, 133, 190, 211, 137, 201, 63, 215, 163, 64, 192, 116, 23, 159, 191, 17, 187, 147, 63, 227, 50, 104, 189, 154, 70, 71, 63, 48, 191, 133, 63, 66, 29, 180, 191, 134, 185, 204, 62, 38, 21, 177, 191, 172, 125, 131, 192, 204, 187, 68, 63, 232, 82, 133, 64, 74, 147, 112, 64, 174, 71, 241, 63, 173, 192, 229, 61, 105, 2, 236, 63, 122, 91, 86, 189, 191, 20, 69, 63, 201, 47, 214, 61, 15, 214, 91, 190, 206, 125, 70, 191, 56, 218, 190, 192, 200, 100, 63, 192, 88, 167, 18, 63, 205, 204, 204, 189, 221, 103, 198, 61, 183, 109, 235, 62, 51, 65, 235, 63, 103, 91, 165, 190, 115, 35, 3, 192, 66, 118, 81, 191, 60, 132, 34, 192, 155, 22, 202, 62, 107, 106, 137, 190, 96, 44, 57, 64, 61, 10, 23, 63, 21, 36, 142, 191, 202, 1, 131, 61, 176, 124, 89, 64, 197, 179, 186, 63, 173, 115, 5, 64, 54, 245, 36, 63, 169, 183, 57, 64, 14, 116, 26, 191, 133, 100, 34, 64, 31, 148, 203, 62, 196, 117, 92, 191, 147, 95, 84, 192, 100, 59, 246, 192, 166, 106, 33, 191, 45, 53, 88, 192, 174, 71, 135, 191, 87, 28, 83, 63, 166, 121, 55, 62, 198, 146, 47, 63, 53, 227, 204, 190, 209, 135, 76, 191, 56, 163, 10, 64, 192, 17, 202, 62, 188, 165, 129, 64, 91, 29, 115, 64, 87, 241, 23, 63, 192, 244, 33, 61, 102, 194, 12, 62, 214, 196, 83, 191, 97, 51, 11, 64, 2, 131, 182, 62, 246, 40, 172, 63, 45, 39, 204, 189, 166, 46, 23, 191, 185, 141, 134, 61, 148, 42, 3, 64, 14, 238, 145, 61, 146, 220, 107, 190, 78, 138, 74, 63, 178, 112, 30, 191, 232, 140, 231, 191, 61, 10, 111, 192, 245, 39, 143, 191, 37, 73, 207, 63, 72, 33, 15, 193, 129, 149, 191, 192, 22, 32, 183, 192, 35, 249, 4, 192, 102, 102, 7, 192, 83, 151, 123, 191, 176, 200, 69, 63, 203, 205, 130, 189, 55, 137, 193, 62, 30, 60, 86, 64, 242, 150, 38, 62, 58, 225, 98, 63, 158, 132, 135, 64, 118, 132, 153, 63, 16, 153, 172, 63, 188, 245, 181, 62, 66, 134, 186, 63, 63, 77, 32, 60, 179, 68, 59, 191, 65, 25, 106, 191, 251, 58, 120, 192, 205, 204, 140, 191, 113, 4, 182, 190, 22, 12, 152, 62, 219, 238, 7, 64, 63, 53, 222, 62, 7, 191, 221, 189, 110, 84, 99, 63, 76, 236, 66, 64, 61, 10, 151, 64, 59, 76, 221, 63, 78, 36, 135, 64, 63, 63, 255, 63, 253, 68, 83, 63, 203, 14, 182, 64, 191, 109, 163, 62, 124, 183, 89, 188, 234, 218, 61, 189, 166, 50, 134, 62, 104, 50, 142, 190, 143, 5, 219, 191, 14, 175, 253, 192, 26, 146, 83, 191, 103, 73, 46, 192, 22, 95, 177, 190, 199, 67, 13, 64, 147, 173, 171, 190, 6, 146, 90, 63, 234, 89, 248, 63, 53, 245, 45, 61, 119, 211, 131, 64, 55, 224, 25, 64, 172, 125, 59, 63, 205, 133, 68, 192, 249, 138, 47, 190, 87, 87, 87, 191, 139, 108, 19, 63, 72, 225, 170, 63, 98, 97, 87, 63, 239, 95, 202, 190, 55, 36, 24, 63, 70, 137, 188, 191, 253, 252, 12, 193, 105, 130, 142, 190, 194, 196, 114, 191, 190, 67, 50, 63, 184, 58, 235, 61, 76, 136, 150, 192, 182, 50, 56, 192, 22, 141, 128, 192, 240, 167, 102, 191, 147, 123, 14, 63, 68, 139, 236, 190, 192, 10, 252, 190, 211, 142, 10, 192, 205, 246, 3, 191, 20, 205, 151, 62, 231, 102, 56, 63, 243, 238, 188, 63, 102, 93, 144, 64, 111, 18, 3, 189, 57, 240, 63, 64, 137, 136, 152, 63, 119, 80, 34, 63, 219, 140, 138, 189, 54, 207, 8, 190, 174, 71, 44, 192, 56, 175, 38, 192, 28, 185, 145, 190, 139, 108, 151, 191, 6, 42, 10, 64, 5, 80, 206, 61, 65, 16, 3, 191, 208, 61, 174, 191, 140, 16, 137, 63, 45, 196, 204, 192, 173, 200, 9, 192, 47, 120, 147, 64, 189, 51, 60, 64, 41, 92, 61, 64, 223, 248, 129, 190, 101, 135, 57, 64, 154, 153, 156, 63, 231, 66, 210, 190, 104, 104, 104, 63, 201, 153, 10, 64, 249, 2, 17, 63, 204, 240, 184, 62, 109, 160, 19, 191, 41, 220, 21, 64, 173, 120, 143, 62, 160, 204, 114, 190, 254, 18, 235, 192, 104, 216, 181, 63, 122, 53, 97, 192, 201, 47, 161, 192, 208, 40, 127, 62, 42, 252, 125, 191, 54, 14, 32, 64, 253, 126, 38, 189, 236, 81, 116, 64 };

float forecast_humidity_predict(const float *features, int32_t features_length) {

        float avg = 0;

        avg += forecast_humidity_tree_0(features, features_length); 
    avg += forecast_humidity_tree_1(features, features_length); 
    avg += forecast_humidity_tree_2(features, features_length); 
    avg += forecast_humidity_tree_3(features, features_length); 
    avg += forecast_humidity_tree_4(features, features_length); 
    avg += forecast_humidity_tree_5(features, features_length); 
    avg += forecast_humidity_tree_6(features, features_length); 
    avg += forecast_humidity_tree_7(features, features_length); 
    avg += forecast_humidity_tree_8(features, features_length); 
    avg += forecast_humidity_tree_9(features, features_length); 
    avg += forecast_humidity_tree_10(features, features_length); 
    avg += forecast_humidity_tree_11(features, features_length); 
    avg += forecast_humidity_tree_12(features, features_length); 
    avg += forecast_humidity_tree_13(features, features_length); 
    avg += forecast_humidity_tree_14(features, features_length); 
    avg += forecast_humidity_tree_15(features, features_length); 
    avg += forecast_humidity_tree_16(features, features_length); 
    avg += forecast_humidity_tree_17(features, features_length); 
    avg += forecast_humidity_tree_18(features, features_length); 
    avg += forecast_humidity_tree_19(features, features_length); 
    avg += forecast_humidity_tree_20(features, features_length); 
    avg += forecast_humidity_tree_21(features, features_length); 
    avg += forecast_humidity_tree_22(features, features_length); 
    avg += forecast_humidity_tree_23(features, features_length); 
    avg += forecast_humidity_tree_24(features, features_length); 
    avg += forecast_humidity_tree_25(features, features_length); 
    avg += forecast_humidity_tree_26(features, features_length); 
    avg += forecast_humidity_tree_27(features, features_length); 
    avg += forecast_humidity_tree_28(features, features_length); 
    avg += forecast_humidity_tree_29(features, features_length); 
    avg += forecast_humidity_tree_30(features, features_length); 
    avg += forecast_humidity_tree_31(features, features_length); 
    avg += forecast_humidity_tree_32(features, features_length); 
    avg += forecast_humidity_tree_33(features, features_length); 
    avg += forecast_humidity_tree_34(features, features_length); 
    avg += forecast_humidity_tree_35(features, features_length); 
    avg += forecast_humidity_tree_36(features, features_length); 
    avg += forecast_humidity_tree_37(features, features_length); 
    avg += forecast_humidity_tree_38(features, features_length); 
    avg += forecast_humidity_tree_39(features, features_length); 
        
        return avg/40;
    }
    
#endif // FORECAST_HUMIDITY_H
