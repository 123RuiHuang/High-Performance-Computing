#!/bin/bash

grep "Point:f" p1.out | awk -F':' '{printf("%s\t%s\t%s\n",$3,$4,$5);}' > p1.f.data && \
grep "Point:m1" p1.out | awk -F':' '{printf("%s\t%s\t%s\n",$3,$4,$5);}' > p1.m1.data



