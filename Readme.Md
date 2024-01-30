Currently the the folder graph 500 is newer version , 3.0.1 with energy calculations with aml folder
the code currently has problems in runing the make file in src directory , cd src --> make ,
the problem is with LDFLAGS, which needs to be resolved follwed the steps on the githud issue link:
but could not figure out the reason for the error 

the folder graph500-2.1.4 is version 2.1.4 , which runs smoothly , the folder make folder include statement has to be rewritten , include make.inc-gcc , and drag the make.inc-gcc file out of make-incs folder, the run make in mpi folder , and rename make.mpi as makefile in generator folder and run it these will produce executables which can be changes accordingly 