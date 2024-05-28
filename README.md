# Project Overview

This project is a Qt-based application designed for managing and translating key-value pairs of Crusader Kings , primarily from YAML files. It provides a user-friendly interface to load, display, translate, and save these pairs, integrating advanced translation functionalities and handling specific text patterns.

![image](https://github.com/asmcodersource/CR3Translator/assets/126491635/14d764ed-7a47-4294-8e22-aaabead96093)

## Features

- **File Management**: Load key-value pairs from YAML files, and save the translated pairs back to YAML files.
- **Translation**: Translate values of key-value pairs using a network-based translation server.
- **Pattern Recognition**: Capture and manage specific patterns in the text such as tags, bracketed text, and variable text.
- **Interactive UI**: A clean, interactive user interface using Qt widgets, including a list view for displaying key-value pairs.

## It is important to know
The translator utilizes the lingva.ml service for its translations. In some cases, you may need to manually configure the service address in the program's settings to ensure proper functionality. To find active and working [Lingva services](https://github.com/thedaviddelta/lingva-translate), please visit the Lingva project's page on GitHub. This page provides a list of available instances that you can use if the default one is not working or if you encounter any connectivity issues.
