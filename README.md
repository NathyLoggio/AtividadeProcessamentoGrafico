# Atividade de Processamento Gráfico 
# Aluna: Nathaly Loggiovini
# Lógica implementada

A atividade realizada apresenta uma lógica de animação de sprites 2D utilizando OpenGL, GLFW, GLAD e GLM. A estrutura principal é composta por uma classe Sprite responsável por:

- *Inicialização de Sprites:* Cada sprite é inicializado com um VAO (Vertex Array Object), textura, shader, posição, escala, rotação, quantidade de frames horizontais e verticais (para spritesheets) e FPS da animação.

- *Animação:* A atualização da animação é feita por meio do método update, que avança o frame atual de acordo com o tempo (deltaTime) e a direção informada, permitindo a navegação por diferentes linhas do spritesheet (direções ou ações).

- *Movimentação:* O método move altera a posição do sprite na tela, modificando os valores de position.x e position.y.

- *Renderização:* O método draw é responsável por desenhar o sprite na tela, configurando os uniformes do shader para recortar a subimagem correta do spritesheet, aplicando as transformações de posição, escala e rotação, e renderizando o quad com a textura correspondente.

A lógica permite o controle animado de sprites via spritesheet, facilitando a implementação de personagens com múltiplas direções e animações, além de garantir modularidade e reutilização do código para diferentes sprites.

---

## Observações

- A lógica é facilmente extensível para múltiplos sprites e animações diferentes, bastando instanciar novos objetos Sprite com diferentes parâmetros.
- O código está modularizado para facilitar manutenção e expansão.

## Obrigada