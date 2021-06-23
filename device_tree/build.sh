#!/bin/sh
echo Building device tree overlay...
cpp -nostdinc -I. -undef, -x assembler-with-cpp PL_PARALLEL-00A0.dts > PL_PARALLEL-00A0.dts.tmp
echo Raw dts file created
dtc -I dts -O dtb -o PL_PARALLEL-00A0.dtbo -@ PL_PARALLEL-00A0.dts.tmp
rm PL_PARALLEL-00A0.dts.tmp
echo Device tree overlay building done
