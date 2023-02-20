# Binary-read-and-write-program
University work 

## Base struct
The program offers some functions to data base, at this moment it has a struct to divide in six tips of data:

- long long int for storage the code of remedy
- float to storage the price
- char array with 9 positions to storage if the remedy have amoxicillin(positive or negative)
- char array with 4 positions to storage yes or no, to indicate if the remedy need refrigeration
- char array with 30 positions to storage the stripe of remedy
- int number to storage 1 or 0 (wasn't right wigh bool), this is to say to module !remove and !save

```cpp
  struct base
  {
  long long int code;
  float price;
  char exam[9];
  char condition[4];
  char remedy[30];
  int active = 1;
  };
``` 

## Read module
In this module, the file "base25.csv" is read to convert csv to binary, so, while the archive doesn't reach to the
end, a size of the total data occuped by base is copied to "basebin.bin", using "ios_base::app" to write only in the
final of archive, because, in this loop, the variable basebin open and close the binary archive.

```cpp
void read(base b)
{
    ifstream archive("base25.csv", ios_base::in);

    while (!archive.eof())
    {
        archive >> b.code >> b.price >> b.exam >> b.condition;
        archive.getline(b.remedy, 30);

        ofstream basebin("basebin.bin", ios_base::app | ios_base::binary);
        basebin.write((char *)&b, sizeof(base));
        basebin.close();
    }

    archive.close();
}
```
## Display module
As the name says, this module is responsible for display the content of "basebin.bin" with some cutomizations.
So, with the variable "fin" of type ifstream, the archive basebin.bin is open and the read starts, of course,
the struct "int active" declared before is used now, inside a while, with a condition, because, in the future, will
be used to define if the data has been removed or not.

```cpp
void display(base b) // exibe o arquivo tal que o cont exibirá a posição de cada coluna começando por 1
{
    ifstream fin;
    int counter = 1;

    fin.open("basebin.bin", ios_base::in | ios_base::binary);

    while (fin.read((char *)&b, sizeof(base))) // lê o arquivo binário e imprime ele.
    {
        if (b.active != 0)
        {
            cout << counter << ": \n"
                 << "\nCode: " << b.code << "\nPrice: " << b.price << "\nPrecense of amoxicillin: " << b.exam
                 << "\nNeed refrigeration: " << b.condition << "\nType of remedy: " << b.remedy 
                 << endl
                 << endl
                 << endl;
            counter++;
        }
    }

    fin.close();
}
``` 
On the screen, will be displayed like this(with examples):

  1
  
  Code: 7898930000000
  
  Price: 45.89
  
  Presence of amoxicillin: Positive
  
  Need refrigeration: No
  
  Type of remedy: Black stripe
  

## Add module



  
  






