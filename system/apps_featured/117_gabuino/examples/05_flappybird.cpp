// Play flappy bird in browser, press F1 on LA104 to jump
// Based on Tiny flappy bird by Daniel Bark:
//   https://github.com/danba340/tiny-flappy-bird

#include <library.h>

int main(void)
{
    BIOS::DBG::Print(R"(<canvas id="c" width="400" height="400" style="background:gray"></canvas>)");

    BIOS::DBG::Print(R"(<script>
    context = document.getElementById("c").getContext("2d");
    bird = new Image();
    bird.src = "https://raw.githubusercontent.com/danba340/tiny-flappy-bird/master/bird.png";
    birdX = birdDY = score = bestScore = 0;
    interval = birdSize = pipeWidth = topPipeBottomY = 24;
    birdY = pipeGap = 200;
    canvasSize = pipeX = 400;
    c.onclick = () => (birdDY = 9);
    game = setInterval(() => {
      context.fillStyle = "skyblue";
      context.fillRect(0,0,canvasSize,canvasSize); // Draw sky
      birdY -= birdDY -= 0.5; // Gravity
      context.drawImage(bird, birdX, birdY, birdSize * (524/374), birdSize); // Draw bird
      context.fillStyle = "green";
      pipeX -= 8; // Move pipe
      pipeX < -pipeWidth && // Pipe off screen?
        ((pipeX = canvasSize), (topPipeBottomY = pipeGap * Math.random())); // Reset pipe and randomize gap.
      context.fillRect(pipeX, 0, pipeWidth, topPipeBottomY); // Draw top pipe
      context.fillRect(pipeX, topPipeBottomY + pipeGap, pipeWidth, canvasSize); // Draw bottom pipe
      context.fillStyle = "black";
      context.fillText(score++, 9, 25); // Increase and draw score
      bestScore = bestScore < score ? score : bestScore; // New best score?
      context.fillText('Best: '+bestScore, 9, 50); // Draw best score
      (((birdY < topPipeBottomY || birdY > topPipeBottomY + pipeGap) && pipeX < birdSize * (524/374))// Bird hit pipe?
       || birdY > canvasSize) && // Bird falls off screen
      ((birdDY = 0), (birdY = 200), (pipeX = canvasSize), (score = 0)); // Bird died
    }, interval)
  </script>)");

    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        if (key == BIOS::KEY::EKey::Enter)
            BIOS::DBG::Print(R"(<script>birdDY=9</script>)");
    }
    BIOS::DBG::Print(R"(<script>clearInterval(game)</script>)");
    return 0;
}
