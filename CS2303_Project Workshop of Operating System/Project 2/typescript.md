# Typescript of Project 2

UNIkeEN

## Stooge Farmers Problem

### Usage

```shell
./LCM <Maxnum>
```

This command can output a possible solution of stooge farmers problem.

`Maxnum` ：Max number of unfilled holes.

If executed successfully, the program will output as:

```shell
Larry digs another hole #1
Moe plants a seed at hole #1
Curly fills a planted hole #1
Larry digs another hole #2
Moe plants a seed at hole #2
Curly fills a planted hole #2
...
```

It will end when 100 steps are run ( so have at least 100 holes filled by Curly ), such as:

```shell
...
Curly fills a planted hole #97
Larry digs another hole #100
Moe plants a seed at hole #100
Curly fills a planted hole #98
Curly fills a planted hole #99
Curly fills a planted hole #100
End Run
```

### Error Prompt

* Wrong number of command line arguments.

  The argument of this program are required. If the number of arguments is incorrect, you will see the following prompt:

  ```shell
  Error: Invalid command line arguments.
  ```

* Invalid maxnum.

  The input `Maxnum` needs to be a positive integer. If it does not meet the criteria, you will receive the following error prompt:

  ```shell
  Error: Invalid number of max holes.
  ```

  (Non integers input greater than 1 are acceptable because atoi will round them up.)


## The Faneuil Hall Problem

### Usage

```shell
./faneuil
```

This command can output a possible solution of faneuil hall problem.

If executed successfully, the program will output as:

```shell
Immigrant #0 enter
Immigrant #0 checkIn
Immigrant #0 sitDown
Immigrant #1 enter
...
Immigrant #3 sitDown
Spectator #0 enter
Spectator #0 spectate
Judge #0 enter
Judge #0 confirm the immigrant #0 
Immigrant #0 getCertificate
Judge #0 confirm the immigrant #1 
Judge #0 confirm the immigrant #2 
Immigrant #1 getCertificate
Immigrant #2 getCertificate
Judge #0 confirm the immigrant #3 
Judge #0 leave
Immigrant #3 getCertificate
Immigrant #3 leave
Spectator #0 leave
...
```

Program works in an infinite loop. Press Ctrl+Z to exit.

There are random delays between steps. 

There are differences in the probability of randomly generating three types of threads in order to be closer to the real situation.

