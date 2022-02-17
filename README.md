# AZH Worker

The scripts in this repo do following works:
- Generate processes
- Calculate the cross section
- Generate events
- Reconstruct the events according to $A\to ZH, H\to t\bar t, Z\to \ell\ell$ topology (or the flipped one)
    - top quarks decay leptonically
- Generate the 2-D distribution for $m_{tt}$-$m_{Ztt}$
- Calculate the log-likelihood between signal and background.

## Results

The full results are located in the `RESULTS` folder. Some basic information is recorded in `json` files. The cross section quoted is in `pb` and is for $gg\to S, S\to Z S', S'\to t\bar t$, with $S,S'=A,H$.

- `202109090949_COMP` keeps the results for $A\to ZH$ studies with triangle, box and interferences separately.
- `202109091918_COMP` keeps the results for $H\to ZA$ studies with triangle, box and interferences separately.
- Other folders there are kept for my own record and have nothing to do with the final results.
- In the `Distributions` subfolder, the `mtt_mztt_xxxxx.txt` files contain the distribution information for each processes.
    - The meaning for each column can be read from the first row.
    - The distributions in $m_{tt}$-$m_{Ztt}$ are generated with $20\times20=400$ bins.
    - The last two columns are the same for all triangle, box and interferences.
    - The `NLL` column represents $2\Delta\ln L$ when combining triangle, box and interference for the particular parameters against the background.
    - `MU` column represents the upper bound on the signal strength.

## References

If you use any result in this repository, please cite [arXiv: 2110.xxxxx](https://arxiv.org/abs/2110.xxxxx).
