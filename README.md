# Docs

```
  void collage(Rgb** arr, Config* cfg, int H, int W, int M, int N){
	Rgb** arr_zoomed = Zoom(arr, cfg, H, W, M, N);
    int tmp_i = 0;
    int tmp_j = 0;
    H = H - (H%M);
    W = W - (W%N);
    int H_zoomed = H / M;
    int W_zoomed = W / N;
    for(int i = 0; i < H ; i ++){
      for(int j = 0; j < W; j ++){
        tmp_i = i % H_zoomed;
        tmp_j = j % W_zoomed;
            arr[i][j] = arr_zoomed[tmp_i][tmp_j];
      }
    }
    for(int i = 0; i < (H / M); i++)
      free(arr_zoomed[i]);
    free(arr_zoomed);
  }
```
