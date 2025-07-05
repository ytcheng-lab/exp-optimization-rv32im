main()
{
    Load N, i[], o_g[], o_t[], buf[]

    // --- Test baseline ---
    start_cycle = read_cycle()
    exp_golden(i, o_g, N)
    baseline_cycles = read_cycle() - start_cycle

    // --- Test speed up version ---
    #ifdef SPEEDUP
        start_cycle = read_cycle()
        exp_taylor3(i, o_t, N)
        optimized_cycles = read_cycle() - start_cycle
    #endif

    // --- Print result ---
    print N
    print baseline_cycles
    #ifdef SPEEDUP
        print optimized_cycles
    #endif

    // --- Verify ---
    #ifdef CHECK
        max_err = 0
        for i in 0..N:
            err = abs(o_t[i] - o_g[i])
            if err > max_err:
                max_err = err
        print max_err
    #endif

    while(1);
}