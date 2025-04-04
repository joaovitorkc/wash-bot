# 🤖 Robô Coletor de Sujeiras

[![Feito com C](https://img.shields.io/badge/Made%20with-C-blue)](https://en.wikipedia.org/wiki/C_(programming_language))  

🚀 Uma simulação em C de um robô que percorre um mapa 8x8 coletando sujeiras e retornando à estação... ou não.

---

## 📚 Sobre o Projeto

Este programa é uma simulação simples em C onde o usuário monta um ambiente com paredes, sujeiras e uma estação de recarga.  
O robô (`@`) percorre o mapa buscando automaticamente a sujeira mais próxima utilizando distância euclidiana, limpa o local e retorna à estação. Caso alguma sujeira seja inalcançável (por estar isolada por paredes), o robô para e aguarda sem encerrar o programa.

> 💡 Ideal para estudos de lógica, simulação de agentes autônomos e movimentação em matrizes.

---

## 🧠 Lógica do Robô

- O robô **busca a sujeira mais próxima** (usando distância euclidiana).
- Se movimenta apenas por caminhos livres, sujeiras ou a estação.
- Paredes (`|---|`) são intransponíveis.
- Se não houver caminho até uma sujeira, ele **para onde está** e **o programa continua rodando**.

---

## ⚙️ Como Executar

1. Compile com:
   ```bash
   gcc matrix.c -o matrix -lm
   ```
   
2. Execute:
   ```bash
   ./matrix
   ``` 
