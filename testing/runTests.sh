echo "Running input file 1"
./encrypt2 infile1 outfile1test

echo "Output Difference:"
diff outfile1 outfile1test

echo "Running input file 2"
./encrypt2 infile2 outfile2test

echo "Output Difference:"
diff outfile2 outfile2test
