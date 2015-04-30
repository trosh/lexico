#!/bin/bash
#@ class            = clallmds+
#@ job_name         = lex
#@ total_tasks      = 1
#@ node             = 1
#@ wall_clock_limit = 00:01:00
#@ as_limit         = 28gb
#@ node_usage       = not_shared
#@ output           = $(job_name).log
#@ error            = $(job_name).err
#@ job_type         = mpich
#@ environment      = COPY_ALL
#@ queue
module load intel openmpi
export OMP_NUM_THREADS=8
mpirun -x OMP_NUM_THREADS ./datdecoupe -d noacfiles/*
