# 📁 Simple Organizer

Organizador de arquivos com interface gráfica feita em **C++ com Qt6**.
Permite criar regras de extensão → pasta destino e organizar diretórios com um clique.

---

## ⚙️ Requisitos

- CMake ≥ 3.16
- Qt6 (Core + Widgets)
- GCC ou Clang com suporte a C++17
- Git (opcional)

---

## 🐧 Arch Linux (KDE / Wayland)

Instala as dependências:

```bash
sudo pacman -S qt6-base cmake ninja gcc
```

---

## 🔨 Compilar e Rodar

Clone ou copie os ficheiros do projeto numa pasta, depois:

```bash
cd simple_organizer
mkdir build && cd build
cmake .. -G Ninja
ninja
./simple_organizer
```

---

## 🗂️ Estrutura do Projeto

```
simple_organizer/
├── CMakeLists.txt
├── main.cpp
├── mainwindow.h / mainwindow.cpp   ← Interface Qt
├── organizer.h / organizer.cpp     ← Lógica de negócio
└── paths.pth                       ← Regras salvas (gerado automaticamente)
```

---

## 🧭 Como Usar

1. **Adicionar Regra** — escolhe uma categoria (imagens, vídeos, etc), seleciona as extensões e define a pasta destino
2. **Remover Regra** — seleciona uma linha na tabela e clica em remover
3. **Organizar Pasta** — escolhe uma pasta e o programa move os ficheiros automaticamente conforme as regras
4. **Buscar** — filtra as regras pela extensão em tempo real

As regras ficam salvas no ficheiro `paths.pth` na mesma pasta do executável.

---

## 🛠️ Abrir no CLion

1. **File → Open** → seleciona a pasta do projeto
2. O CLion detecta o `CMakeLists.txt` automaticamente
3. Aguarda o CMake carregar e compila normalmente

> **Dependências necessárias no CLion (Arch):**
> ```bash
> sudo pacman -S qt6-base cmake ninja gcc gdb
> ```

---

## 📝 Notas

- O ficheiro `paths.pth` é criado automaticamente na primeira regra adicionada
- Em Wayland (KDE), a interface roda nativamente sem configuração extra
- Ficheiros com nomes duplicados no destino são sobrescritos silenciosamente (comportamento atual)
