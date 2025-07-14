
void buscarPorAutor();
void listarPorAno();
void rankingLivros();
void alugarLivro();
void devolverLivro();#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100
#define MAX_EMAIL 80
#define ARQ_LIVROS "livros.txt"
#define ARQ_USUARIOS "usuarios.txt"

typedef struct {
    char titulo[100];
    char autor[100];
    int ano;
    int alugado; // 0 = não, 1 = sim
    int vezes_lido;
} Livro;

typedef struct {
    char nome[100];
    char email[MAX_EMAIL];
    char senha[20];
} Usuario;

Livro livros[MAX];
int totalLivros = 0;

// Funções auxiliares
bool emailValido(const char *email);
bool emailJaExiste(const char *email);
void salvarLivros();
void carregarLivros();
void cadastrarLivro();
void listarLivros();
void editarLivro();
void excluirLivro();
void buscarPorTitulo();
void menu();
void cadastrarUsuario();

bool login() {
    char usuario[MAX_EMAIL], senha[20]; 
    do {
        printf("Digite o e-mail: ");
        scanf(" %[^\n]", usuario);
        getchar(); 
        printf("Digite a senha: ");
        scanf(" %[^\n]", senha);
        getchar(); 

        FILE *f = fopen(ARQ_USUARIOS, "r");
        if (!f) {
            printf("Erro: Não foi possível abrir o arquivo de usuários.\n");
            return false;
        }

        Usuario u;
        while (fscanf(f, " %[^;];%[^;];%[^\n]\n", u.nome, u.email, u.senha) != EOF) {
            if (strcmp(u.email, usuario) == 0 && strcmp(u.senha, senha) == 0) {
                fclose(f);
                printf("\nBem-vindo(a) à Biblioteca Digital, %s!\n", u.nome);
                return true;
            }
        }
        fclose(f);
        printf("Usuário ou senha inválidos. Tente novamente.\n");
    } while (1);
}

bool emailValido(const char *email) {
    if (strlen(email) >= MAX_EMAIL) {
        printf("Erro: Email muito longo (máximo %d caracteres).\n", MAX_EMAIL - 1);
        return false;
    }
    if (!strchr(email, '@')) {
        printf("Erro: Email inválido (falta '@').\n");
        return false;
    }
    return true;
}

bool emailJaExiste(const char *email) {
    FILE *f = fopen(ARQ_USUARIOS, "r");
    if (!f) {
       
        return false;
    }

    Usuario u;
    while (fscanf(f, " %[^;];%[^;];%[^\n]\n", u.nome, u.email, u.senha) != EOF) {
        if (strcmp(u.email, email) == 0) {
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

void cadastrarUsuario() {
    Usuario u;

    printf("Nome: ");
    scanf(" %[^\n]", u.nome);
    getchar(); 

    do {
        printf("Email: ");
        scanf(" %[^\n]", u.email);
        getchar(); 

        if (!emailValido(u.email)) {
            
            continue;
        }
        if (emailJaExiste(u.email)) {
            printf("Email já cadastrado. Tente outro.\n");
            continue;
        }
        break; // Email válido e não existente
    } while (1);

    printf("Senha: ");
    scanf(" %[^\n]", u.senha);
    getchar(); 

    FILE *f = fopen(ARQ_USUARIOS, "a");
    if (!f) {
        printf("Erro ao abrir o arquivo de usuários para escrita.\n");
        return;
    }
    fprintf(f, "%s;%s;%s\n", u.nome, u.email, u.senha);
    fclose(f);
    printf("Usuário cadastrado com sucesso!\n");
}

void salvarLivros() {
    FILE *f = fopen(ARQ_LIVROS, "w");
    if (!f) {
        printf("Erro ao abrir o arquivo de livros para escrita.\n");
        return;
    }
    for (int i = 0; i < totalLivros; i++) {
        fprintf(f, "%s;%s;%d;%d;%d\n", livros[i].titulo, livros[i].autor, livros[i].ano, livros[i].alugado, livros[i].vezes_lido);
    }
    fclose(f);
}

void carregarLivros() {
    FILE *f = fopen(ARQ_LIVROS, "r");
    if (!f) return; 
    while (fscanf(f, " %[^;];%[^;];%d;%d;%d\n", livros[totalLivros].titulo, livros[totalLivros].autor, &livros[totalLivros].ano, &livros[totalLivros].alugado, &livros[totalLivros].vezes_lido) != EOF) {
        totalLivros++;
        if (totalLivros >= MAX) { 
            printf("Aviso: Limite de livros atingido ao carregar. Nem todos os livros foram carregados.\n");
            break;
        }
    }
    fclose(f);
}

void cadastrarLivro() {
    if (totalLivros >= MAX) {
        printf("Limite de livros atingido!\n");
        return;
    }
    printf("Título: ");
    scanf(" %[^\n]", livros[totalLivros].titulo);
    getchar(); 
    printf("Autor: ");
    scanf(" %[^\n]", livros[totalLivros].autor);
    getchar(); 
    printf("Ano: ");
    scanf("%d", &livros[totalLivros].ano);
    getchar(); 
    livros[totalLivros].alugado = 0;
    livros[totalLivros].vezes_lido = 0;
    totalLivros++;
    salvarLivros();
    printf("Livro cadastrado com sucesso!\n");
}

void listarLivros() {
    if (totalLivros == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Livros ---\n");
    for (int i = 0; i < totalLivros; i++) {
        printf("[%d] %s - %s (%d) | Alugado: %s | Lido: %d vezes\n", i + 1, livros[i].titulo, livros[i].autor, livros[i].ano,
               livros[i].alugado ? "Sim" : "Não", livros[i].vezes_lido);
    }
}

void editarLivro() {
    listarLivros();
    if (totalLivros == 0) return; 

    int id;
    printf("Digite o número do livro para editar: ");
    scanf("%d", &id);
    getchar(); 
    id--; 

    if (id < 0 || id >= totalLivros) {
        printf("ID inválido.\n");
        return;
    }
    printf("Novo título: ");
    scanf(" %[^\n]", livros[id].titulo);
    getchar(); 
    printf("Novo autor: ");
    scanf(" %[^\n]", livros[id].autor);
    getchar();
    printf("Novo ano: ");
    scanf("%d", &livros[id].ano);
    getchar(); 
    salvarLivros();
    printf("Livro editado com sucesso!\n");
}

void excluirLivro() {
    listarLivros();
    if (totalLivros == 0) return; // Se não há livros, não há o que excluir

    int id;
    printf("Digite o número do livro para excluir: ");
    scanf("%d", &id);
    getchar(); 
    id--; 

    if (id < 0 || id >= totalLivros) {
        printf("ID inválido.\n");
        return;
    }
    for (int i = id; i < totalLivros - 1; i++) {
        livros[i] = livros[i + 1];
    }
    totalLivros--;
    salvarLivros();
    printf("Livro excluído.\n");
}

void buscarPorTitulo() {
    char busca[100];
    printf("Digite o título para buscar: ");
    scanf(" %[^\n]", busca);
    getchar(); 
    bool encontrado = false;
    for (int i = 0; i < totalLivros; i++) {
        if (strstr(livros[i].titulo, busca)) {
            printf("%s - %s (%d) | Alugado: %s | Lido: %d vezes\n", livros[i].titulo, livros[i].autor, livros[i].ano,
                   livros[i].alugado ? "Sim" : "Não", livros[i].vezes_lido);
            encontrado = true;
        }
    }
    if (!encontrado) {
        printf("Nenhum livro encontrado com o título '%s'.\n", busca);
    }
}

void buscarPorAutor() {
    char busca[100];
    printf("Digite o autor para buscar: ");
    scanf(" %[^\n]", busca);
    getchar(); 
    bool encontrado = false;
    for (int i = 0; i < totalLivros; i++) {
        if (strstr(livros[i].autor, busca)) {
            printf("%s - %s (%d) | Alugado: %s | Lido: %d vezes\n", livros[i].titulo, livros[i].autor, livros[i].ano,
                   livros[i].alugado ? "Sim" : "Não", livros[i].vezes_lido);
            encontrado = true;
        }
    }
    if (!encontrado) {
        printf("Nenhum livro encontrado do autor '%s'.\n", busca);
    }
}

void listarPorAno() {
    int ano;
    printf("Digite o ano para listar: ");
    scanf("%d", &ano);
    getchar(); 
    bool encontrado = false;
    for (int i = 0; i < totalLivros; i++) {
        if (livros[i].ano == ano) {
            printf("%s - %s (%d) | Alugado: %s | Lido: %d vezes\n", livros[i].titulo, livros[i].autor, livros[i].ano,
                   livros[i].alugado ? "Sim" : "Não", livros[i].vezes_lido);
            encontrado = true;
        }
    }
    if (!encontrado) {
        printf("Nenhum livro encontrado para o ano '%d'.\n", ano);
    }
}

void rankingLivros() {
    if (totalLivros == 0) {
        printf("\nNenhum livro cadastrado para gerar ranking.\n");
        return;
    }
    
    printf("\n--- Ranking de Livros Mais Lidos ---\n");
    for (int i = 0; i < totalLivros; i++) {
        printf("%s - Lido %d vezes\n", livros[i].titulo, livros[i].vezes_lido);
    }
}

void alugarLivro() {
    listarLivros();
    if (totalLivros == 0) return; // Se não há livros, não há o que alugar

    int id;
    printf("Digite o número do livro para alugar: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada
    id--; // Ajusta para índice base 0

    if (id < 0 || id >= totalLivros) {
        printf("ID inválido.\n");
        return;
    }

    if (livros[id].alugado) {
        printf("Livro '%s' já está alugado!\n", livros[id].titulo);
    } else {
        livros[id].alugado = 1;
        livros[id].vezes_lido++;
        salvarLivros();
        printf("Livro '%s' alugado com sucesso!\n", livros[id].titulo);
    }
}

void devolverLivro() {
    listarLivros();
    if (totalLivros == 0) return; // Se não há livros, não há o que devolver

    int id;
    printf("Digite o número do livro para devolver: ");
    scanf("%d", &id);
    getchar(); 
    id--; 

    if (id < 0 || id >= totalLivros) {
        printf("ID inválido.\n");
        return;
    }

    if (!livros[id].alugado) {
        printf("Livro '%s' não está alugado.\n", livros[id].titulo);
    } else {
        livros[id].alugado = 0;
        salvarLivros();
        printf("Livro '%s' devolvido com sucesso!\n", livros[id].titulo);
    }
}

void menu() {
    int op;
    do {
        printf("\n===== MENU BIBLIOTECA DIGITAL =====\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Listar Livros\n");
        printf("3. Editar Livro\n");
        printf("4. Excluir Livro\n");
        printf("5. Buscar Livro por Título\n");
        printf("6. Buscar Livro por Autor\n");
        printf("7. Listar Livros por Ano\n");
        printf("8. Ranking dos Mais Lidos\n");
        printf("9. Alugar Livro\n");
        printf("10. Devolver Livro\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &op);
        getchar(); 

        switch (op) {
            case 1: cadastrarLivro(); break;
            case 2: listarLivros(); break;
            case 3: editarLivro(); break;
            case 4: excluirLivro(); break;
            case 5: buscarPorTitulo(); break;
            case 6: buscarPorAutor(); break;
            case 7: listarPorAno(); break;
            case 8: rankingLivros(); break;
            case 9: alugarLivro(); break;
            case 10: devolverLivro(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida! Tente novamente.\n");
        }
    } while (op != 0);
}

int main() {
    carregarLivros();
    printf("Bem-vindo(a) ao sistema da Biblioteca Digital!\n");

    int escolha;
    do {
        printf("1 - Fazer login\n2 - Cadastrar novo usuário\nEscolha: ");
        scanf("%d", &escolha);
        getchar(); 
        if (escolha != 1 && escolha != 2) {
            printf("Opção inválida. Por favor, escolha 1 ou 2.\n");
        }
    } while (escolha != 1 && escolha != 2);


    if (escolha == 2) {
        cadastrarUsuario();
    }

    // Tenta fazer login após cadastro ou se a opção inicial foi login
    if (login()) {
        menu();
    } else {
        printf("Falha no login. Encerrando o programa.\n");
    }

    return 0;
}
