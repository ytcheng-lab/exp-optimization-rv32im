void exp_golden(float* in, float* out, int N)
{
    for i in 0..N:
        out[i] = exp(in[i])
}

float expth3(float x)
{
    return ((x * 1/6 + 0.5) * x + 1) * x + 1
}

void exp_taylor3(float* in, float* out, int N)
{
    for i in 0..N:
        out[i] = expth3(in[i])
}

float LUT1[40] = { ... }

float expth_LUT1(float x)
{
    n = int(x)
    exp_n = LUT1[n]
    f = x - n
    exp_f = ((f * 1/6 + 0.5) * f + 1) * f + 1
    return exp_n * exp_f
}

void softmax_expthLUT1(float* in, float* out, float* buf, int N)
{
    sum = 0
    for i in 0..N:
        x_ = in[i] + 10
        buf[i] = expth_LUT1(x_)
        sum += buf[i]
    for i in 0..N:
        out[i] = buf[i] / sum
}
