# Typescript of Project 1

UNIkeEN

## Copy

### Usage

```shell
./Copy <InputFile> <OutputFile> <BufferSize>
```

This command can copy InputFile to OutputFile.

If the path of OutputFile exists but the file itself does not exist, it will be created.

For example, we can use the following command to write a file `sample.out` with the same content as `sample.in`.

```shell
./Copy sample.in sample.out 128
```

If executed successfully, the program will output the execution time such as:

```shell
Time used: 0.354000 milliseconds.
```

### Error Prompt

* Wrong number of command line arguments.

  All three arguments of this program are required. If the number of arguments is incorrect, you will see the following prompt:

  ```shell
  Error: Invalid command line arguments.
  ```

* Invalid buffer size.

  The input buffer size needs to be a positive integer. If it does not meet the criteria, you will receive the following error prompt:

  ```shell
  Error: Invalid buffer size.
  ```

  (Non integers input greater than 1 are acceptable because atoi will round them up.)

* Fail to create pipe

  ```shell
  Error: Pipe failed.
  ```

* Fail to fork the sub process.

  ```shell
  Error: Failed to fork.
  ```

* Fail to create buffer.(May be unable to allocate due to insufficient space)

  ```shell
  Error: Could not create buffer.
  ```

* Fail to open source or destnation file

  When the InputFile does not exist, or the path before the OutputFile file name does not exist, the following error output will be obtained:

  ```shell
  Error: Could not open source/destnation file <InputFile>/<outputFile>
  ```

  When the InputFile does not exist and the OutputFile exists or be successfully created, the OutputFile will be empty.

### Performance Test

You can use `test.py` if you have Python environment to generate fixed-size files using random characters, run the `./Copy` with different buffer size, extract the output time and draw the figure.

The output of test script is the LATEX table of average execution time.

```shell
> python test.py

\begin{tabular}{l|ccccccccccccc}
\toprule
 & \multicolumn{13}{c}{Buffer Size}\\
\cmidrule(lr){2-14}
File Size (MiB) & 4 & 8 & 16 & 32 & 64 & 128 & 256 & 512 & 1024 & 2048 & 4096 & 8192 & 16384 \\
\midrule
1 MiB & 58.56 & 34.74 & 15.81 & 10.11 & 7.46 & 6.18 & 5.44 & 3.93 & 4.53 & 5.13 & 2.00 & 3.69 & 1.63 \\
4 MiB & 57.06 & 30.99 & 16.71 & 10.05 & 7.58 & 6.24 & 3.62 & 2.56 & 4.47 & 4.19 & 1.89 & 1.86 & 1.88 \\
...
256 MiB & 63.94 & 31.00 & 16.52 & 7.86 & 4.81 & 3.03 & 2.38 & 1.98 & 1.78 & 1.70 & 1.51 & 1.29 & 1.17 \\
\bottomrule
\end{tabular}
```

## Shell

### Usage

#### Run the server

```shell
./shell <Port>
```

This command can run the shell server on Port. If executed successfully, the program will output nothing initially.

#### Connect to the server

When the server is running, we can use the following command to connect to server. The server support multiple clients to connect at the same time.

```bash
telnet localhost <Port>
```

If executed successfully, the client will be shown as a system shell. For example:

```shell
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
unikeen@ubuntu:/home/unikeen/Code/OS_Project_CS2303/Project_1/Shell> 
```

> I designed a colored prompt for my shell. See the report for details

#### Run commands

Now you can use the client terminal as a system shell. For example, input:

```shell
ls -l | wc
```

The server terminal will output what it received:

```shell
The server terminal will output:
```

The client terminal will output the result of the command:

```shell
      4      29     181
```

#### CD and exit

You can use `cd` command in my shell just as the system shell, the prompt will change. Different client can have different working directories.

When you input `exit` command in the client, the connection of this client will be closed by the host, Such as:

```
unikeen@ubuntu:/home/unikeen/Code/OS_Project_CS2303/Project_1/Shell> exit
Connection closed by foreign host.
```

#### Local Mode

There's another local version of shell that satisfy basic requirements of the problem. It cannot running as server. You can use `./shell_local`  to run it.

#### Attention

If you input the command with `sudo`, you need to input the password on server terminal.

### Error Prompt

* Fail to connect between server and client.

  ```shell
  accept: <error description>
  ```

* Fail to fork the sub processes.

  ```shell
  fork: <error description>
  ```

* Wrong directory for the `cd` command.

  ```shell
  Error: Unable to change directory
  ```

* Invalid commands or invalid arguments of the command.

  ```shell
  Execvp: <error description> 
  或 <command name>: <error description> (e.g: top: unrecognized field name 'oo')
  ```

* No other command after the last pipe symbol in input.

  ```shell
  Wrong pipe parameters.
  ```


## Matrix

### Usage

There are three ways to run this task：

```shell
./Single # run in the single thread mode
./multi  # run in the multi thread mode, read input from '’'\data.in', and output to '\data.out'.
./multi <Size> # generate Size*Size matrix A and do A*A
```

If executed successfully, the program will output the execution time such as:

```shell
Time used: 0.652000 millisecond
```

### Error Prompt

The error prompt is similar to Copy, and an error is reported when a file cannot be found or multiple threads cannot be created/executed.
