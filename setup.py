from setuptools import Extension, setup

ext = Extension(
    name='zengl_export',
    sources=['./zengl-export.cpp'],
    define_macros=[('PY_SSIZE_T_CLEAN', None)],
)

with open('README.md') as readme:
    long_description = readme.read()

setup(
    name='zengl-export',
    version='0.1.0',
    ext_modules=[ext],
    install_requires=['zengl>=1.9.3'],
    license='MIT',
    python_requires='>=3.6',
    platforms=['any'],
    description='Dump ZenGL context to raw OpenGL calls',
    long_description=long_description,
    long_description_content_type='text/markdown',
    author='Szabolcs Dombi',
    author_email='cprogrammer1994@gmail.com',
    url='https://github.com/szabolcsdombi/zengl-export/',
    project_urls={
        'Documentation': 'https://zengl-export.readthedocs.io/',
        'Source': 'https://github.com/szabolcsdombi/zengl-export/',
        'Bug Tracker': 'https://github.com/szabolcsdombi/zengl-export/issues/',
    },
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
        'Topic :: Games/Entertainment',
        'Topic :: Multimedia :: Graphics',
        'Topic :: Multimedia :: Graphics :: 3D Rendering',
        'Topic :: Scientific/Engineering :: Visualization',
    ],
)
