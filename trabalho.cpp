#include <iostream>
#include <fstream>
using namespace std;

struct base // struct do trabalho.
{
    long long int codigo;
    float preco;
    char exame[9];
    char condicao[4];
    char medicamento[30];
    int active = 1;
};

void read(base b) // roda no começo do programa (uma única vez). Lê a base em csv e escreve um arquivo binário com os conteúdos delas.
{

    ifstream arquivo("base25.csv", ios_base::in);

    while (!arquivo.eof()) // enquanto o arquivo não acabar, o programa continua lendo o arquivo.
    {
        arquivo >> b.codigo >> b.preco >> b.exame >> b.condicao;
        arquivo.getline(b.medicamento, 30);

        ofstream basebin("basebin.bin", ios_base::app | ios_base::binary); // escreve o arquivo em binário.

        basebin.write((char *)&b, sizeof(base));
        basebin.close();
    }

    arquivo.close();
}

void display(base b) // exibe o arquivo tal que o cont exibirá a posição de cada coluna começando por 1
{   
    ifstream fin;
    int cont = 1;

    fin.open("basebin.bin", ios_base::in | ios_base::binary);

    while (fin.read((char *)&b, sizeof(base))) // lê o arquivo binário e imprime ele.
    {
        if (b.active != 0)
        {
            cout << cont << ": \n"
                 << "\nCódigo: " << b.codigo << "\nPreço: " << b.preco << "\nPresença de Amoxilina: " << b.exame
                 << "\nHá necessidade de refrigeração: " << b.condicao << "\nTipo do medicamento: " << b.medicamento << endl
                 << endl
                 << endl;
            cont++;
        }
    }
    fin.close();
}

void add(base b) // adiciona uma entrada na base no arquivo binário
{
    ofstream edit;

    edit.open("basebin.bin", ios_base::app | ios_base::binary); // abre o arquivo e adiciona coisas no fim dele com o app

    cout << "Adicione a entrada que gostaria abaixo. Por favor, tente seguir as sugestões e evitar caracteres especiais, pois estes não são bem interpretados pela leitura.\n";

    cout << "\nCódigo: ";
    cin >> b.codigo;

    cout << "\nPreço: ";
    cin >> b.preco;

    cout << "\nPresença de Amoxilina (Positiva/Negativa): ";
    cin >> b.exame;

    cout << "\nHá necessidade de refrigeração?(Sim/Nao): ";
    cin >> b.condicao;
    cin.ignore();

    cout << "\nTipo do medicamento (Tarja): ";
    
    cin.getline(b.medicamento, 29);

    int i = 0;
    while (b.medicamento[i] != '\0')
    {
        i++;
    }

    char remedio[30];
    remedio[0] = ' ';
    for(int j = 1; j <= i; j++)
    {
        remedio[j] = b.medicamento[j-1];
    }
    for(int j = 0; j <= i; j++)
    {
        b.medicamento[j] = remedio[j];
    }

    edit.write((char *)&b, sizeof(base)); // escreve o input do usuário no final do arquivo.

    system("clear");

    cout << "Dados adicionados com sucesso.\n\n";
    edit.close();
}

void remove(base b) // remove uma entrada
{
    int pos;
    long long int search;
    bool deactivated = false;
    bool found = false;

    cout << "Por favor, insira o código do medicamento o qual quer remover: ";
    cin >> search;

    fstream deactivate("basebin.bin", ios_base::in | ios_base::out | ios_base::binary);

    while (deactivate.read((char *)&b, sizeof(base)) && deactivated == false) // lê o arquivo enquanto deactivated for falso. DEVERIA parar assim que for true
    {
        if (b.codigo == search && b.active != 0) // se o código for igual ao que o usuário digitou, entra nesse if
        {
            b.active = 0;                               // marca o active (uma das variáveis do struct) como 0 . Basicamente, ia fazer depois pra que caso esse active seja 0, aquela linha for ignorada. Isso não acontece ainda.
            pos = deactivate.tellp();                   // pos = linha a qual o if entrou
            deactivate.seekp(pos - sizeof(base));       // seekp vai até a linha que o if entrou
            deactivate.write((char *)&b, sizeof(base)); // arquivo escreve as variáveis do struct, mas dessa vez active == 0, não 1.

            deactivated = true; // para o while
            found = true;
        }
    }
    deactivate.close();
    if (found == true)
    {
        cout << "\nRemovido com sucesso!";
    }
    else
        cout << "\nEste código não foi encontrado. Cheque se digitou corretamente.";
}

void section(base b)
{

    ifstream sect;
    int end = 0;
    sect.open("basebin.bin", ios_base::in | ios_base::binary);

    int range1, range2;
    cout << "que seção gostaria de imprimir?\n\n";
    cout << "Linha onde a seção desejada começa: ";

    cin >> range1;

    cout << "\nLinha onde a seção desejada termina: ";
    cin >> range2;
    cout << "\n";

    int cont = 1;

    while (end == 0&&sect.read((char *)&b, sizeof(base)))
    {
        if (range2<cont)//para o while se passar do range
        {
            end = 1;
        }
        if (cont >= range1 && cont <= range2)
        {

            cout << cont << ": \n"
                 << "\nCódigo: " << b.codigo << "\nPreço: " << b.preco << "\nPresença de Amoxilina: " << b.exame
                 << "\nHá necessidade de refrigeração: " << b.condicao << "\nTipo do medicamento: " << b.medicamento << endl
                 << endl
                 << endl;
        }
        cont++;
    }
}

void save(base b, char key) // salva em cpp quando o programa for encerrado, se o usuário quiser.
{

    cout << "Gostaria de salvar o arquivo em formato CSV? (y/n)";
    cin >> key;

    if (key == 'y')
    {
        cout << "\nDigite o nome do arquivo em que deseja salvar" << endl;
        cout << "obs: caso queira manipular o arquivo novo futuramente, digite o nome padrão do arquivo(base25.csv)" << endl;
        string arquivoNew;
        cin >> arquivoNew;
        ifstream finalRead;
        ofstream saveCSV(arquivoNew);

        finalRead.open("basebin.bin", ios_base::in | ios_base::binary);

        finalRead.read((char *)&b, sizeof(base));
        if (b.active == 1)
        {
            saveCSV
             << b.codigo << " " << b.preco << " " << b.exame << " " << b.condicao << b.medicamento;
        }

        while (finalRead.read((char *)&b, sizeof(base))) // lê o arquivo binário e imprime ele num arquivo csv.
        {
            if(b.active == 1)
            {
                saveCSV << "\n"
                    << b.codigo << " " << b.preco << " " << b.exame << " " << b.condicao << b.medicamento;
            }
        }

        finalRead.close();
        saveCSV.close();

        system("clear");
        cout << "Salvo com sucesso!\n\n";
    }

    else
        system("clear");
}

void orderCode(base b, bool codeWasSorted, bool priceWasSorted) // ordenar pelo código
{
    ifstream sizeB;
    sizeB.open("basebin.bin", ios_base::in | ios_base::binary);
    int size = 0;

    if (codeWasSorted == true)
    {
        cout << "\nO arquivo acaba de ser ordenado de acordo com código, em ordem crescente.\n";
    }

    else
    {
        while (sizeB.read((char *)&b, sizeof(base)))
        {
            size++;
        }

        sizeB.close();

        base vet[size];
        ifstream assign;
        assign.open("basebin.bin", ios_base::in | ios_base::binary);
        int i = 0;

        while (assign.read((char *)vet, size * sizeof(base)))
        {
            assign >> vet[i].codigo;
            assign >> vet[i].preco;
            assign >> vet[i].exame;
            assign >> vet[i].condicao;
            assign.getline(vet[i].medicamento, 30);
            assign >> vet[i].active;
            i++;
        }

        assign.close();

        int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
        int pos_gap = 8;

        while (gaps[pos_gap] > size)
        {
            pos_gap--;
        }

        base value;
        int j;

        while (pos_gap >= 0)
        {
            int gap = gaps[pos_gap];

            for (int i = gap; i < size; i++)
            {
                value = vet[i];
                j = i;
                while ((j >= gap) and (value.codigo < vet[j - gap].codigo))
                {
                    vet[j] = vet[j - gap];
                    j = j - gap;
                }
                vet[j] = value;
            }
            pos_gap--;
        }

        remove("basebin.bin");

        ofstream fout("basebin.bin", ios_base::out | ios_base ::binary);
        fout.write((char *)vet, size * sizeof(base));

        fout.close();
        codeWasSorted = true;
        priceWasSorted = false;

        cout << "\nO arquivo acaba de ser ordenado de acordo com código, em ordem crescente.\n";
    }
}

void orderPrice(base b, bool codeWasSorted, bool priceWasSorted) // ordenar pelo preço
{
    ifstream sizeB;
    sizeB.open("basebin.bin", ios_base::in | ios_base::binary);
    int size = 0;

    if (priceWasSorted == true)
    {
        cout << "\nO arquivo acaba de ser ordenado de acordo com preço, em ordem crescente.\n";
    }
    else
    {

        while (sizeB.read((char *)&b, sizeof(base)))
        {
            size++;
        }

        sizeB.close();

        base vet[size];
        ifstream assign;
        assign.open("basebin.bin", ios_base::in | ios_base::binary);
        int i = 0;

        while (assign.read((char *)vet, size * sizeof(base)))
        {
            assign >> vet[i].codigo;
            assign >> vet[i].preco;
            assign >> vet[i].exame;
            assign >> vet[i].condicao;
            assign.getline(vet[i].medicamento, 30);
            assign >> vet[i].active;
            i++;
        }

        assign.close();

        int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
        int pos_gap = 8;

        while (gaps[pos_gap] > size)
        {
            pos_gap--;
        }

        base value;
        int j;

        while (pos_gap >= 0)
        {
            int gap = gaps[pos_gap];

            for (int i = gap; i < size; i++)
            {
                value = vet[i];
                j = i;
                while ((j >= gap) and (value.preco < vet[j - gap].preco))
                {
                    vet[j] = vet[j - gap];
                    j = j - gap;
                }
                vet[j] = value;
            }
            pos_gap--;
        }

        remove("basebin.bin");

        ofstream fout("basebin.bin", ios_base::out | ios_base::binary);
        fout.write((char *)vet, size * sizeof(base));

        fout.close();
        priceWasSorted = true;
        codeWasSorted = false;

        cout << "\nO arquivo acaba de ser ordenado de acordo com preço, em ordem crescente.\n";
    }
}

void searchPrice(bool priceWasSorted, bool codeWasSorted, base b)
{
    float preco1; //preço em float que gera complicações na hora de comparar

    int left = 0, right = -1;

    cout << "Por favor, digite o preço da entrada que está procurando: ";
    cin >> preco1;
    int preco = preco1*100; //multiplicando o preço por 100 para comparar com inteiro
    if (priceWasSorted == false)
    {
        orderPrice(b, codeWasSorted, priceWasSorted);
    }

    ifstream file("basebin.bin", ios_base::in | ios_base::binary);

    // encontrar o final do arquivo
    file.seekg(0, ios_base::end);
    right = file.tellg() / sizeof(base);

    // busca binaria
    while (left <= right)
    {
        int mid = (left + right) / 2;
        file.seekg(mid * sizeof(base));
        file.read((char *)&b, sizeof(base));
        int precoint = b.preco*100;
        if (precoint == preco and b.active != 0)
        {
            cout << "Código: " << b.codigo << endl;
            cout << "Preço: " << b.preco << endl;
            cout << "Presença de amoxilina: " << b.exame << endl;
            cout << "Necessidade de refrigeração: " << b.condicao << endl;
            cout << "Tipo do medicamento: " << b.medicamento << endl;
            file.close();
            return;
        }
        else if (precoint < preco)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    cout << "\nDesculpe, não encontramos esta entrada." << endl;
    file.close();
}

void searchCode(bool priceWasSorted, bool codeWasSorted, base b)
{
    long long int codigo;

    int left = 0, right = -1;

    cout << "Por favor, digite o código da entrada que está procurando: ";
    cin >> codigo;

    if (codeWasSorted == false)
    {
        orderCode(b, codeWasSorted, priceWasSorted);
    }

    ifstream file("basebin.bin", ios_base::in | ios_base::binary);

    // encontrar o final do arquivo
    file.seekg(0, ios_base::end);
    right = file.tellg() / sizeof(base);

    // busca binaria
    while (left <= right)
    {
        int mid = (left + right) / 2;
        file.seekg(mid * sizeof(base));
        file.read((char *)&b, sizeof(base));
        if (b.codigo == codigo && b.active != 0)
        {
            cout << "\nCódigo: " << b.codigo << endl;
            cout << "Preço: " << b.preco << endl;
            cout << "Presença de amoxilina: " << b.exame << endl;
            cout << "Necessidade de refrigeração: " << b.condicao << endl;
            cout << "Tipo do medicamento: " << b.medicamento << endl;
            file.close();
            return;
        }
        else if (b.codigo < codigo)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    cout << "\nDesculpe, não encontramos esta entrada." << endl;
    file.close();
}

int main()
{
    if("basebin.bin")
    {
        remove ("basebin.bin");
    }

    base b;
    char key;
    int input;
    bool codeWasSorted;
    bool priceWasSorted;

    read(b); // chama a função "read", que lê o csv e converte ele pra binário.

    while (key != 'n') // enquanto o usuário não digitar "n" no fim do programa, o programa retorna ao início para que outra operação seja realizada.
    {

        cout << "Bem vindo(a) ao programa!\nO que gostaria de fazer?\n\n";
        cout << "1) Exibir o arquivo.\n";
        cout << "2) Editar o arquivo. \n";
        cout << "3) Ordenar o arquivo.\n";
        cout << "4) Buscar uma entrada.\n";
        cout << "(Digite o número da operação que deseja realizar, e pressione \"Enter\" para confirmar.)\n";

        cin >> input;    // pega o input digitado.
        system("clear"); // limpa o terminal (apenas estético).

        if (input == 1) // o programa passa pra esse bloco de if else depois de pegar o input, dependendo do que for digitado.
        {
            cout << "Exibir uma sessão ou todo o arquivo?\n\n";
            cout << "1) Exibir uma sessão.\n";
            cout << "2) Exibir todo o arquivo.\n";

            cin >> input;
            system("clear");

            if (input == 1)
            {
                section(b);
            }
            else if (input == 2)
            {
                display(b);
            }
            else
            {
                cout << "Por favor, digite um número válido.\n";
            }
        }

        else if (input == 2)
        {
            cout << "O que gostaria de editar?\n\n";
            cout << "1) Remover uma entrada\n";
            cout << "2) Adicionar uma entrada\n";
            cout << "(Digite o número da operação que deseja realizar, e pressione \"Enter\" para confirmar.)\n";

            cin >> input;
            system("clear");

            if (input == 1)
            {
                remove(b);
            }
            else if (input == 2)
            {
                add(b);
            }
            else
            {
                cout << "Por favor, digite um número válido.\n";
            }
        }
        else if (input == 3)
        {
            cout << "Como gostaria de ordenar o arquivo?\n\n";
            cout << "1) Ordenar por código de barras.\n";
            cout << "2) Ordenar por preço.\n";

            cout << "(Digite o número da operação que deseja realizar, e pressione \"Enter\" para confirmar.)\n";

            cin >> input;
            system("clear");

            if (input == 1)
            {
                orderCode(b, priceWasSorted, codeWasSorted);
            }
            else if (input == 2)
            {
                orderPrice(b, priceWasSorted, codeWasSorted);
            }
            else
            {
                cout << "Por favor, digite um número válido.\n";
            }
        }
        else if (input == 4)
        {
            cout << "Como gostaria de realizar a busca pelo arquivo?\n\n";
            cout << "1) Buscar por preço.\n";
            cout << "2) Buscar por código.\n";
            cout << "(Digite o número da operação que deseja realizar, e pressione \"Enter\" para confirmar.)\n";

            cin >> input;
            system("clear");

            if (input == 1)
            {
                searchPrice(priceWasSorted, codeWasSorted, b);
            }
            else if (input == 2)
            {
                searchCode(priceWasSorted, codeWasSorted, b);
            }
            else
            {
                cout << "Por favor, digite um número válido.\n";
            }
        }

        else
        {
            cout << "Por favor, digite um número válido.\n";
        }

        cout << "\nDeseja voltar ao início? (y/n) ";

        cin >> key;

        system("clear");
    }

    save(b, key);

    cout << "Programa encerrado.\n"; // imprime quando o programa sair do bloco while (quando o usuário digitar "n")

    return 0;
}