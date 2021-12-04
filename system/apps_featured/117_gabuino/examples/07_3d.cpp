#include <library.h>

int main(void)
{
    BIOS::DBG::Print(R"(<canvas id="c" style="width:400px; height:400px;"></canvas>

<script type="module">
import * as THREE from 'https://threejsfundamentals.org/threejs/resources/threejs/r132/build/three.module.js';
rot = {x:0, y:0, z:0};

function main() {
  const k = 0.05;

  const canvas = document.querySelector('#c');
  const renderer = new THREE.WebGLRenderer({canvas});
  const camera = new THREE.PerspectiveCamera(75, 3, 0.1, 5);
  camera.position.z = 1.5;
  const scene = new THREE.Scene();
  const geometry = new THREE.BoxGeometry(1, 0.3, 1);

  const cubes = [];
  const loader = new THREE.TextureLoader();
  const materials = [
    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-1.jpg')}),
    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-2.jpg')}),
    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-3.jpg')}),
    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-4.jpg')}),
    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-5.jpg')}),
    new THREE.MeshBasicMaterial({map: loader.load('https://threejsfundamentals.org/threejs/resources/images/flower-6.jpg')}),
  ];
  
  const cube = new THREE.Mesh(geometry, materials);
  scene.add(cube);
  cubes.push(cube);

  function resizeRendererToDisplaySize(renderer) {
    const canvas = renderer.domElement;
    const width = canvas.clientWidth;
    const height = canvas.clientHeight;
    const needResize = canvas.width !== width || canvas.height !== height;
    if (needResize) {
      renderer.setSize(width, height, false);
    }
    return needResize;
  }

  function render(time) {
    if (resizeRendererToDisplaySize(renderer)) {
      const canvas = renderer.domElement;
      camera.aspect = canvas.clientWidth / canvas.clientHeight;
      camera.updateProjectionMatrix();
    }

    cubes.forEach((cube, ndx) => {
      cube.rotation.x = cube.rotation.x * (1-k) + rot.x * k;
      cube.rotation.y = cube.rotation.y * (1-k) + rot.y * k;
      cube.rotation.z = cube.rotation.z * (1-k) + rot.z * k;
    });

    renderer.render(scene, camera);
    requestAnimationFrame(render);
  }

  requestAnimationFrame(render);
}

main();
</script>
)");

    int x = 1, z = 0;
    BIOS::KEY::EKey key;
    while ((key = BIOS::KEY::GetKey()) != BIOS::KEY::EKey::Escape)
    {
        switch (key)
        {
            case BIOS::KEY::F1: x=0; z=0; break;
            case BIOS::KEY::Left: z--; break;
            case BIOS::KEY::Right: z++; break;
            case BIOS::KEY::Up: x--; break;
            case BIOS::KEY::Down: x++; break;
        }
        BIOS::SYS::DelayMs(100);
        BIOS::DBG::Print(R"(<script>rot = {y:Math.sin(%d/1000), x:%d/2, z:%d/2}</script>)", 
            BIOS::SYS::GetTick(), x, z);
    }
    return 0;
}
