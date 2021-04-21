echo "Running input file 1"
./encrypt infile1 testOutput1

echo "Output-1 Difference:"
diff outfile1 testOutput1

echo "Running input file 2"
./encrypt infile2 testOutput2

echo "Output-2 Difference:"
diff outfile2 testOutput2
