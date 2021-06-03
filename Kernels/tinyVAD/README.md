# A voice activity detector, designed for the lowest computation number
  
  With tunable sensitivity, and designed for mono 48kHz 16bits/sample
  Introduces 100ms latency, works with any size of input samples

  void vad_estimation(long *command, long *vad, short *inputData, long nbSamples)

## Algorithm description
     The input samples are high-pass filtered 
     The energy of the results is used to estimate peaks and floor noise
     When the ratio envelope/floor_noise exceeds a threshold  
     a counter is used to wait for confirmation 
