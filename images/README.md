![Project's banner](HILO_banner.png)

## 1.0 Files

| Files and Directories     | Description                             |
|---------------------------|-----------------------------------------|
|development_breadboard.png | Image used in the main README.md file   |
|HILO_banner.png            | The title of every README.md file       |
|Makefile                   | Automation tool for diagram generating  |
|[Makefile.conf]            | Optional Makefile's configuration file  |
|overviewDiagram.puml       | Diagram used in the main README.md file |
|overviewDiagram.png        | Generated image                         |

## 2.0 Decription
In order to provide README files where they are more comfortable to read, I choice to use Markup-Down (MD) language instead row
ASCII text. This type of README files are more compatible with GITHUB and the MD language has become an appreciated standart for
the documentation files. 

One of the more appreciated MD's feature is the possibility to embedd images, diagrams, links... into the document. But in order
to keepm an ordered tree, I choise to store all images in this folder.

## 2.1 Diagram generating
In order to include the diagram's file in the project's file versioning I use the [plantuml](https://plantuml.com) app. It allows
me to create complicated graphic diagrams starting by a sequence of ASCII instructions. The 
[offical manual](https://plantuml.com/guide#goog_rewarded) is also available.

## 2.2 Images on GIT??!!!??
I know, many people are drawing a terrible expressions in their face, now! GIT is not made to store images. I completely
agree with you. My solution would be to include puml file only in git, and store all static images in a web-area. Unfortunately
GITHB does not provide a web-space!! Someone use a proper branch for the image files. But it looks like Walt Disney's catoon
Pippo (Goofy in EN) when he sweep the dust under the carpet, IMHO. So, I have to wait that GitHUB implements a PlantUML engine in
its Markdown language interpreter.

However, I will be happy to evaluate every possible suggestion

A sentence by ChatGPT-5.1 about PlantUML: "___Il futuro si sta allineando a PlantUML, non il contrario.___"; translated version
"___So the future is aligning with PlantUML, not the other way around.___"
by ChatGPT (20/11/2025)
