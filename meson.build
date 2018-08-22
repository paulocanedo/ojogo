project('ojogo', ['cpp', 'c'],        version: '0.1.0',
  meson_version: '>= 0.40.0',
  default_options : ['cpp_std=c++14']
)

glfw_dep = dependency('glfw3')
gl_dep = dependency('GL')
x11_dep = dependency('x11')
thread_dep = dependency('threads')
xrandr_dep = dependency('xrandr')
xi_dep = dependency('xi')
freetype2 = dependency('freetype2')

add_project_arguments([
  '-I' + meson.build_root(),
  '-lX11 -lXrandr -lXi -ldl',
], language: 'cpp')

subdir('src')

executable('ojogo', [
  './src/main.cpp',
  './src/glad.c',
  './src/stb_image.c',
  './src/engine/Game.cpp',
  './src/engine/Sprite.cpp',
  './src/engine/Texture.cpp',
  './src/engine/animation/AnimationMultiTexture.cpp',
  './src/engine/animation/AnimationTranslate.cpp',
],
	link_args: '-ldl',
	dependencies: [
		glfw_dep, gl_dep, thread_dep, x11_dep, xrandr_dep, xi_dep, freetype2
	])
