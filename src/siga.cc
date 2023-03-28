#include <string>
#include <sstream>

using namespace std;

#include "siga.h"
#include "config.h"

Siga::Siga()
{
    this->arquivo_nome = "";
    this->n_estudantes = 0;
}

void Siga::SetDatabase(string arquivo_dados_estudante)
{

    this->arquivo_nome = INPUT_DATA_DIR+arquivo_dados_estudante;

    // Obter numero de estudantes no arquivo
    // Se o arquivo não existir, o numero de estudantes é zero
    // Se o arquivo existir, o numero de estudantes é o numero de registros no arquivo
    //  o numero de estudantes é armazenado no atributo n_estudantes

   

    this->file_stream = fstream(this->arquivo_nome, ios::in | ios::out | ios::binary);
    cout << "SIGA: Inicializado com sucesso" << endl;
    
    // arquivo nao existe ainda
    if(!this->file_stream.is_open())
    {
    
        // cria um arquivo vazio
        ofstream out(arquivo_nome, ios::binary);
        out.close();
        // reabre para leitura e escrita
        this->file_stream = fstream(this->arquivo_nome, ios::in | ios::out | ios::binary);
        if(!this->file_stream.is_open())
        {
            cout << "SIGA: Erro ao criar arquivo de dados de estudante" << endl;
            return;
        }
    
    }


    this->file_stream.seekg(0, this->file_stream.end);
    int length = this->file_stream.tellg();
    this->file_stream.seekg(0, this->file_stream.beg);
    this->n_estudantes = length / sizeof(Estudante);
    
    cout << this->n_estudantes << " registros de estudantes" << endl;
}


void Siga::LerCSV(string arquivo_csv)
{   
    //Implementação leitura de arquivo CSV
    // Passos:
    // Abrir arquivo CSV

    string arquivo_csv_path = INPUT_DATA_DIR+arquivo_csv;
    ifstream csv_file;
    csv_file.open(arquivo_csv_path);
    if(!csv_file.is_open())
    {
        cout << "Erro ao abrir arquivo CSV" << endl;
        return;
    }
    // Ler cabeçalho
    string line;
    getline(csv_file, line);

    // Para cada linha de dados
    while(getline(csv_file, line))
    {
        // Ler um estudante do arquivo CSV
        Estudante est;
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        est.TrocarNome(token.c_str());
        getline(ss, token, ',');
        est.TrocarMatricula(stoi(token));
        getline(ss, token, ',');
        est.TrocarAnoIngresso(stoi(token));
        getline(ss, token, '\n');
        est.TrocarIRA(stof(token));

        // Escrever o objeto estudante no arquivo binário
        this->EscrevaEstudante(this->n_estudantes, est);

        // Atualize o numero de estudantes no atributo n_estudantes
        this->n_estudantes++;
    }
    // Fim-Para
    // Fechar arquivo CSV
    csv_file.close();

    cout << this->n_estudantes << " registros de estudantes" << endl;
}


int  Siga::PesquisaPorMatricula(int matricula)
{
   this->file_stream.seekg(0, this->file_stream.beg);

    for (int i = 0; i < this->ObterNumeroEstudantes(); i++)
    {
        Estudante est;
        this->file_stream.read((char *)&est, sizeof(Estudante));
        if (est.ObterMatricula() == matricula)
        {
            cout << "Posiçao estudante:" << i << endl;
            return i;
        }
    }

    cout << "o estudante nao foi encontrado!" << endl;
    return -1;
}
        
void Siga::AdicionaEstudante(Estudante est)
{
    if (this->PesquisaPorMatricula(est.ObterMatricula()) != -1)
    {
        cout << "O estudante ja foi cadastrado" << endl;
        return;
    }

    this->EscrevaEstudante(this->n_estudantes, est);
    this->n_estudantes++;
    
}
  
Estudante Siga::ObterEstudante(int idx)
{
    Estudante est;
    
    this->file_stream.seekg(0, this->file_stream.beg);

    this->file_stream.seekg(idx * sizeof(Estudante), this->file_stream.beg);

    this->file_stream.read((char *)&est, sizeof(Estudante));

    return est;
}
        
void Siga::SalvaCSV(string arquivo_csv)
{
    string arquivo_csv_path = INPUT_DATA_DIR + arquivo_csv;

    ofstream csv_file;

    csv_file.open(arquivo_csv_path);

    if (!csv_file.is_open())
    {
        cout << "Não foi possivel abrir o arquivo" << endl;
        return;
    }

    csv_file << "Nome,Matricula,AnoIngresso,IRA" << endl;

    this->file_stream.seekg(0, this->file_stream.beg);

    for (int i = 0; i < this->ObterNumeroEstudantes(); i++)
    {
        Estudante est;
        this->file_stream.read((char *)&est, sizeof(Estudante));
        csv_file << est.ObterNome() << "/" << est.ObterMatricula() << "/" << est.ObterAnoIngresso() << "/" << est.ObterIRA() << endl;
    }

    csv_file.close();
    cout << this->n_estudantes << "estudantes registrados" << endl;

    return;
   
}
        
        
void Siga::AlteraCadastroEstudante(int idx, Estudante est)
{
    this->file_stream.seekg(0, this->file_stream.beg);
    this->file_stream.seekg(idx * sizeof(Estudante), this->file_stream.beg);
    this->file_stream.write((char *)&est, sizeof(Estudante));

    return;
}
        
Siga::~Siga()
{
    this->file_stream.close();
}

int Siga::ObterNumeroEstudantes()
{
    return this->n_estudantes;
}


void Siga::LeiaEstudante(int idx, Estudante &est)
{
    this->file_stream.seekg(idx * sizeof(Estudante), this->file_stream.beg);
    this->file_stream.read((char *)&est, sizeof(Estudante));
}

void Siga::EscrevaEstudante(int idx, Estudante est)
{
    this->file_stream.seekp(idx * sizeof(Estudante), this->file_stream.beg);
    this->file_stream.write((char *)&est, sizeof(Estudante));
}