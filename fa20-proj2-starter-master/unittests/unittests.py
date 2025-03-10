from unittest import TestCase
from framework import AssemblyTest, print_coverage


class TestAbs(TestCase):
    def test_zero(self):
        t = AssemblyTest(self, "abs.s")
        # load 0 into register a0
        t.input_scalar("a0", 0)
        # call the abs function
        t.call("abs")
        # check that after calling abs, a0 is equal to 0 (abs(0) = 0)
        t.check_scalar("a0", 0)
        # generate the `assembly/TestAbs_test_zero.s` file and run it through venus
        t.execute()

    def test_one(self):
        # same as test_zero, but with input 1
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", 1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    def test_minus_one(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    def test_max_int(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", 2147483647)
        t.call("abs")
        t.check_scalar("a0", 2147483647)
        t.execute()

    def test_min_int(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -2147483648)
        t.call("abs")
        t.check_scalar("a0", 2147483648)
        t.execute()

    def test_random(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -42)
        t.call("abs")
        t.check_scalar("a0", 42)
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("abs.s", verbose=False)

class TestRelu(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [1, 0, 3, 0, 5, 0, 7, 0, 9])
        # generate the `assembly/TestRelu_test_simple.s` file and run it through venus
        t.execute()

    def test_all_negative(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([-1, -2, -3, -4, -5, -6, -7, -8, -9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [0, 0, 0, 0, 0, 0, 0, 0, 0])
        # generate the `assembly/TestRelu_test_all_negative.s` file and run it through venus
        t.execute()

    def test_all_positive(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [1, 2, 3, 4, 5, 6, 7, 8, 9])
        # generate the `assembly/TestRelu_test_all_positive.s` file and run it through venus
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("relu.s", verbose=False)


class TestArgmax(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        # raise NotImplementedError("TODO")
        # TODO
        # load address of the array into register a0
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a0", array0)
        # TODO
        # set a1 to the length of the array
        t.input_scalar("a1", len(array0))
        # TODO
        # call the `argmax` function
        t.call("argmax")
        # TODO
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 8)
        # TODO
        # generate the `assembly/TestArgmax_test_simple.s` file and run it through venus
        t.execute()

    def test_random(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        # raise NotImplementedError("TODO")
        # TODO
        # load address of the array into register a0
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 7, 6, 5, 4, 3, 2, 1])
        t.input_array("a0", array0)
        # TODO
        # set a1 to the length of the array
        t.input_scalar("a1", len(array0))
        # TODO
        # call the `argmax` function
        t.call("argmax")
        # TODO
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 8)
        # TODO
        # generate the `assembly/TestArgmax_test_random.s` file and run it through venus
        t.execute()

    def test_sample(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        array0 = t.array([21, 65, 32, 45, 76, 98, 12, 8, 99, 0, 23, 54, 76, 34, 23, 12])
        # load address of the array into register a0
        t.input_array("a0", array0)
        # set a1 to the length of the array
        t.input_scalar("a1", len(array0))
        # call the `argmax` function
        t.call("argmax")
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 8)
        # generate the `assembly/TestArgmax_test_sample.s` file and run it through venus
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("argmax.s", verbose=False)


class TestDot(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        # raise NotImplementedError("TODO")
        # TODO
        # load array addresses into argument registers
        array0 = t.array([1, 2, 3])
        array1 = t.array([4, 5, 6])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # TODO
        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
        # TODO
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 32)
        # TODO
        t.execute()

    def test_random(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        # raise NotImplementedError("TODO")
        # TODO
        # load array addresses into argument registers
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([9, 8, 7, 6, 5, 4, 3, 2, 1])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # TODO
        # load array attributes into argument registers
        t.input_scalar("a2", 5)
        t.input_scalar("a3", 2)
        t.input_scalar("a4", 2)
        # TODO
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 85)
        # TODO
        t.execute()

    def test_sample(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([9, 8, 7, 6, 5, 4, 3, 2, 1])
        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)
        t.input_scalar("a4", 3)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 54)
        t.execute()

    def test_sample2(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([9, 8, 7, 6, 5, 4, 3, 2, 1])
        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)
        t.input_scalar("a4", 2)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 72)
        t.execute()
    
    def test_sample3(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 2)
        t.input_scalar("a4", 1)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 22)
        t.execute()

    def test_length_mismatch(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # load array attributes into argument registers
        t.input_scalar("a2", 0)
        t.input_scalar("a3", 3)
        t.input_scalar("a4", 2)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.execute(code=75)

    def test_stride_mismatch(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 0)
        t.input_scalar("a4", 3)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.execute(code=76)

    @classmethod
    def tearDownClass(cls):
        print_coverage("dot.s", verbose=False)


class TestMatmul(TestCase):

    def do_matmul(self, m0, m0_rows, m0_cols, m1, m1_rows, m1_cols, result, code=0):
        t = AssemblyTest(self, "matmul.s")
        # we need to include (aka import) the dot.s file since it is used by matmul.s
        t.include("dot.s")

        # create arrays for the arguments and to store the result
        array0 = t.array(m0)
        array1 = t.array(m1)
        array_out = t.array([0] * len(result))

        # load address of input matrices and set their dimensions
        # raise NotImplementedError("TODO")
        # TODO
        # load address of output array
        t.input_array("a0", array0)
        t.input_scalar("a1", m0_rows)
        t.input_scalar("a2", m0_cols)
        t.input_array("a3", array1)
        t.input_scalar("a4", m1_rows)
        t.input_scalar("a5", m1_cols)
        t.input_array("a6", array_out)
        # TODO

        # call the matmul function
        t.call("matmul")

        # check the content of the output array
        t.check_array(array_out, result)
        # TODO

        # generate the assembly file and run it through venus, we expect the simulation to exit with code `code`
        t.execute(code=code)

    def test_simple(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150]
        )
        self.do_matmul(
            [15, 21, 27, 36, 51, 66, 57, 81, 20, 1, 6, 8], 3, 4,
            [1, 2, 3, 4, 5, 6, 7, 8], 4, 2,
            [465, 564, 1101, 1356, 109, 144]
        )
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 0, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150], 72
        )
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 0, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150],73
        )
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 2, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150], 74
        )

    @classmethod
    def tearDownClass(cls):
        print_coverage("matmul.s", verbose=False)


class TestReadMatrix(TestCase):

    def do_read_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")

        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
        # raise NotImplementedError("TODO")
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        # TODO

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        t.check_array(rows, [3])
        t.check_array(cols, [3])
        t.check_array_pointer("a0", [1, 2, 3, 4, 5, 6, 7, 8, 9])
        # TODO

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        
    def do_read_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/m0.bin")

        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
        # raise NotImplementedError("TODO")
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        # TODO

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        t.check_array(rows, [4])
        t.check_array(cols, [2])
        t.check_array_pointer("a0", [11, -10, 13, 10, -23, -6, -22, 10])
        # TODO

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)

    def test_malloc_error(self):
        self.do_read_matrix(fail='malloc', code=88)
    
    def test_fopen_error(self):
        self.do_read_matrix(fail='fopen', code=90)

    def test_fread_error(self):
        self.do_read_matrix(fail='fread', code=91)
    
    def test_fclose_error(self):
        self.do_read_matrix(fail='fclose', code=92)
    def test_simple(self):
        self.do_read_matrix()

    @classmethod
    def tearDownClass(cls):
        print_coverage("read_matrix.s", verbose=False)


class TestWriteMatrix(TestCase):

    def do_write_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")
        outfile = "outputs/test_write_matrix/student.bin"
        # load output file name into a0 register
        t.input_write_filename("a0", outfile)
        # load input array and other arguments
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a1", array0)
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)
        # raise NotImplementedError("TODO")
        # TODO
        # call `write_matrix` function
        t.call("write_matrix")
        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        if (code != 0):
            return
        # compare the output file against the reference
        t.check_file_output(outfile, "outputs/test_write_matrix/reference.bin")


    def test_simple(self):
        self.do_write_matrix()

    def test_fopen_error(self):
        self.do_write_matrix(fail='fopen', code=93)

    def test_fwrite_error(self):
        self.do_write_matrix(fail='fwrite', code=94)
    
    def test_fclose_error(self):
        self.do_write_matrix(fail='fclose', code=95)

    @classmethod
    def tearDownClass(cls):
        print_coverage("write_matrix.s", verbose=False)


class TestClassify(TestCase):

    def make_test(self):
        t = AssemblyTest(self, "classify.s")
        t.include("argmax.s")
        t.include("dot.s")
        t.include("matmul.s")
        t.include("read_matrix.s")
        t.include("relu.s")
        t.include("write_matrix.s")
        return t

    def test_simple0_input0(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args)

        # compare the output file and
        t.check_file_output(out_file, ref_file)
        t.check_stdout("2")
    
    def test_malloc_error(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=88, fail='malloc')
    
    def test_wrong_arguments(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin"]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=89)

    @classmethod
    def tearDownClass(cls):
        print_coverage("classify.s", verbose=False)


class TestMain(TestCase):

    from unittest import TestCase
from framework import AssemblyTest, print_coverage


class TestAbs(TestCase):
    def test_zero(self):
        t = AssemblyTest(self, "abs.s")
        # load 0 into register a0
        t.input_scalar("a0", 0)
        # call the abs function
        t.call("abs")
        # check that after calling abs, a0 is equal to 0 (abs(0) = 0)
        t.check_scalar("a0", 0)
        # generate the `assembly/TestAbs_test_zero.s` file and run it through venus
        t.execute()

    def test_one(self):
        # same as test_zero, but with input 1
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", 1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()
    
    def test_minus_one(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("abs.s", verbose=False)


class TestRelu(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [1, 0, 3, 0, 5, 0, 7, 0, 9])
        # generate the `assembly/TestRelu_test_simple.s` file and run it through venus
        t.execute()
    
    def test_zero_length(self):
        t = AssemblyTest(self, "relu.s")
        
        # create an array with zero length
        array = t.array([])
        t.input_array("a0", array)
        t.input_scalar("a1", len(array))

        # check relu function raise exception with error code 78
        t.call("relu")
        
        t.execute(code=78)
        
    def test_negative_length(self):
        t = AssemblyTest(self, "relu.s")
        
        # create an array with zero length
        array = t.array([])
        t.input_array("a0", array)
        
        # pass array length with an negative value
        t.input_scalar("a1", -1)

        # check relu function raise exception with error code 78
        t.call("relu")
        t.execute(code=78)

    @classmethod
    def tearDownClass(cls):
        print_coverage("relu.s", verbose=False)


class TestArgmax(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        data = [1, -2, 3, -4, 5, -6, 7, -8, 9, -10, 9, -11]
        array = t.array(data)
        # load address of the array into register a0
        t.input_array("a0", array)
        # set a1 to the length of the array
        t.input_scalar("a1", len(array))
        # call the `argmax` function
        t.call("argmax")
        # check that the register a0 contains the correct output
        t.check_scalar("a0", data.index(max(data)))
        # generate the `assembly/TestArgmax_test_simple.s` file and run it through venus
        t.execute()
    
    def test_zero_length(self):
        t = AssemblyTest(self, "argmax.s")
        
        # create an array with zero length
        array = t.array([])
        t.input_array("a0", array)
        t.input_scalar("a1", len(array))

        # check relu function raise exception with error code 77
        t.call("argmax")
        
        t.execute(code=77)
    
    def test_negative_length(self):
        t = AssemblyTest(self, "argmax.s")
        
        # create an array with zero length
        array = t.array([])
        t.input_array("a0", array)
        
        # pass array length with an negative value
        t.input_scalar("a1", -1)

        # check relu function raise exception with error code 78
        t.call("argmax")
        t.execute(code=77)

    @classmethod
    def tearDownClass(cls):
        print_coverage("argmax.s", verbose=False)


class TestDot(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        data0 = [1, 2, 3, 4, 5]
        data1 = [5, 4, 3, 2, 1]
        assert(len(data0) == len(data1))
        array0 = t.array(data0)
        array1 = t.array(data1)

        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)

        # load array attributes into argument registers
        t.input_scalar("a2", len(data0))
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
    
        # call the `dot` function
        t.call("dot")

        # check the return value
        t.check_scalar("a0", sum(x * y for x, y in zip(data0, data1)))
        t.execute()
    
    def test_stride(self):
        t = AssemblyTest(self, "dot.s")
        data0 = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        data1 = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        assert(len(data0) == len(data1))
        array0 = t.array(data0)
        array1 = t.array(data1)

        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)

        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 2)
    
        # call the `dot` function
        t.call("dot")

        # check the return value
        t.check_scalar("a0", 22)
        t.execute()   
    
    def test_invalid_array_length(self):
        t = AssemblyTest(self, "dot.s")

        data0 = []
        data1 = []
        assert(len(data0) == len(data1))
        array0 = t.array(data0)
        array1 = t.array(data1)

        t.input_array("a0", array0)
        t.input_array("a1", array1)

        t.input_scalar("a2", len(data0))
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
    
        t.call("dot")

        # check the return value
        t.execute(code=75)
    
    def test_invalid_stride_1(self):
        t = AssemblyTest(self, "dot.s")

        data0 = [1]
        data1 = [1]
        assert(len(data0) == len(data1))
        array0 = t.array(data0)
        array1 = t.array(data1)

        t.input_array("a0", array0)
        t.input_array("a1", array1)

        t.input_scalar("a2", len(data0))
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 0)
    
        t.call("dot")

        # check the return value
        t.execute(code=76)
    
    def test_invalid_stride_2(self):
        t = AssemblyTest(self, "dot.s")

        data0 = [1]
        data1 = [1]
        assert(len(data0) == len(data1))
        array0 = t.array(data0)
        array1 = t.array(data1)

        t.input_array("a0", array0)
        t.input_array("a1", array1)

        t.input_scalar("a2", len(data0))
        t.input_scalar("a3", 0)
        t.input_scalar("a4", 1)
    
        t.call("dot")

        # check the return value
        t.execute(code=76)

    @classmethod
    def tearDownClass(cls):
        print_coverage("dot.s", verbose=False)


class TestMatmul(TestCase):

    def do_matmul(self, m0, m0_rows, m0_cols, m1, m1_rows, m1_cols, result, code=0):
        t = AssemblyTest(self, "matmul.s")
        # we need to include (aka import) the dot.s file since it is used by matmul.s
        t.include("dot.s")

        # create arrays for the arguments and to store the result
        array0 = t.array(m0)
        array1 = t.array(m1)
        array_out = t.array([0] * len(result))

        # load address of input matrices and set their dimensions
        t.input_array("a0", array0)
        t.input_scalar("a1", m0_rows)
        t.input_scalar("a2", m0_cols)

        t.input_array("a3", array1)
        t.input_scalar("a4", m1_rows)
        t.input_scalar("a5", m1_cols)

        # load address of output array
        t.input_array("a6", array_out)

        # call the matmul function
        t.call("matmul")

        # check the content of the output array
        t.check_array(array_out, result)

        # generate the assembly file and run it through venus, we expect the simulation to exit with code `code`
        t.execute(code=code)

    def test_simple(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150]
        )
    
    def test_m0_dimension_error(self):
        self.do_matmul(
            m0=[], 
            m0_rows=0, 
            m0_cols=0,
            m1=[1, 2, 3], 
            m1_rows=3,
            m1_cols=1,
            result=[1], # result doesn't matter here because the multiplication is invalid
            code=72
        )
    
    def test_m1_dimension_error(self):
        self.do_matmul(
            m1=[], 
            m1_rows=0, 
            m1_cols=0,
            m0=[1, 2, 3], 
            m0_rows=3,
            m0_cols=1,
            result=[1], # result doesn't matter here because the multiplication is invalid
            code=73
        )
    
    def test_dimension_mismatch(self):
        self.do_matmul(
            m0=[1, 2, 3, 4], 
            m0_rows=2, 
            m0_cols=2,
            m1=[1, 1], 
            m1_rows=1,
            m1_cols=1,
            result=[1], # result doesn't matter here because the multiplication is invalid
            code=74
        )

    @classmethod
    def tearDownClass(cls):
        print_coverage("matmul.s", verbose=False)


class TestReadMatrix(TestCase):

    def do_read_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")

        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
        t.input_array("a1", rows)
        t.input_array("a2", cols)

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        t.check_array(rows, [3])
        t.check_array(cols, [3])
        t.check_array_pointer("a0", [1, 2, 3, 4, 5, 6, 7, 8, 9])

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
    
    def test_simple(self):
        self.do_read_matrix()
    
    def test_malloc_error(self):
        self.do_read_matrix(fail='malloc', code=88)
    
    def test_fopen_error(self):
        self.do_read_matrix(fail='fopen', code=90)

    def test_fread_error(self):
        self.do_read_matrix(fail='fread', code=91)
    
    def test_fclose_error(self):
        self.do_read_matrix(fail='fclose', code=92)

    @classmethod
    def tearDownClass(cls):
        print_coverage("read_matrix.s", verbose=False)


class TestWriteMatrix(TestCase):

    def do_write_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")
        outfile = "outputs/test_write_matrix/student.bin"
        # load output file name into a0 register
        t.input_write_filename("a0", outfile)
        # load input array and other arguments
        array = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a1", array)
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)
        # call `write_matrix` function
        t.call("write_matrix")
        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        # compare the output file against the reference
        if (code != 0):
            return
        t.check_file_output(outfile, "outputs/test_write_matrix/reference.bin")

    def test_simple(self):
        self.do_write_matrix()
    
    def test_fopen_error(self):
        self.do_write_matrix(fail='fopen', code=93)
    
    def test_fwrite_error(self):
        self.do_write_matrix(fail='fwrite', code=94)
    
    def test_fclose_error(self):
        self.do_write_matrix(fail='fclose', code=95)

    @classmethod
    def tearDownClass(cls):
        print_coverage("write_matrix.s", verbose=False)


class TestClassify(TestCase):

    def make_test(self):
        t = AssemblyTest(self, "classify.s")
        t.include("argmax.s")
        t.include("dot.s")
        t.include("matmul.s")
        t.include("read_matrix.s")
        t.include("relu.s")
        t.include("write_matrix.s")
        return t

    def test_simple0_input0(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args)

        # compare the output file and
        t.check_file_output(out_file, ref_file)
        t.check_stdout("2")
    
    def test_malloc_error(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=88, fail='malloc')
    
    def test_wrong_arguments(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin"]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=89)

    @classmethod
    def tearDownClass(cls):
        print_coverage("classify.s", verbose=False)


class TestMain(TestCase):

    def run_main(self, inputs, output_id, label):
        args = [f"{inputs}/m0.bin", f"{inputs}/m1.bin", f"{inputs}/inputs/input0.bin",
                f"outputs/test_basic_main/student{output_id}.bin"]
        reference = f"outputs/test_basic_main/reference{output_id}.bin"
        t = AssemblyTest(self, "main.s", no_utils=True)
        t.call("main")
        t.execute(args=args, verbose=False)
        t.check_stdout(label)
        t.check_file_output(args[-1], reference)

    def test0(self):
        self.run_main("inputs/simple0/bin", "0", "2")

    def test1(self):
        self.run_main("inputs/simple1/bin", "1", "1")
