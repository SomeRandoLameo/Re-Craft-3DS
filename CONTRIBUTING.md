# How can i participate in this project?
We welcome contributions from the community! Here are some ways you can participate in this project:
1. **Report Issues**: If you encounter any bugs or have suggestions for improvements, please open an issue on our GitHub repository. Be sure to provide as much detail as possible to help us understand the problem. 
2. **Submit Pull Requests**: If you would like to contribute code, please fork the repository and create a new branch for your changes. Once you have made your changes, submit a pull request for review. Make sure to follow our coding standards and include tests for any new features or bug fixes.
3. **Graphical Design**: If you have skills in graphic design (Themed around UI Design and Pixel-Art), we welcome contributions to improve the visual aspects of the project, such as logos, icons, and user interface elements.
4. **Documentation**: Good documentation is crucial for any project. If you notice any gaps or areas that could be improved in our documentation, please feel free to contribute by adding or updating content.
5. **Testing**: Help us ensure the quality of our project by testing new features and reporting any issues you find. Your feedback is invaluable in helping us improve the project.
6. **Spread the Word**: Share our project with your network! The more people who know about it, the more contributions we can receive.

## How can i get started with coding?
To get started with coding, please follow these steps:
0. Take a look at our [project board](https://github.com/users/SomeRandoLameo/projects/2). We have a list of issues and features that need attention. Feel free to pick one that interests you.
1. Fork the repository to your own GitHub account.
2. Clone the forked repository to your local machine. To set up a development environment, refer to: [How do i build this rom?](https://github.com/SomeRandoLameo/Re-Craft-3DS?tab=readme-ov-file#how-do-i-build-this-rom) on our Readme.md.
3. Create a new branch for your changes.
4. Make your changes and commit them with clear and descriptive messages.
5. Push your changes to your forked repository.
6. Submit a pull request to the main repository.

## How can i get started with graphical design?
To get started with graphical design contributions, please follow these steps:
1. Fork the repository to your own GitHub account.
2. Clone the forked repository to your local machine.
3. Use LibreSprite/Aseprite to create or modify graphical assets found in [the "mockups" directory](https://github.com/SomeRandoLameo/Re-Craft-3DS/tree/main/mockups).
4. Create a new mockup or duplicate an existing one to work on.
5. Save your changes in the appropriate format and a propper timestamp (.ase for in-progress mockups, .png for final renders).
6. if your Mockup is complete, export every view as a separate .png file in a logical directory structure. (/menus/main_menu/, /ui/inventory/, etc.)
7. Make your changes and commit them with clear and descriptive messages since we cannot see pixels that you modified
8. Push your changes to your forked repository.
9. Submit a pull request to the main repository.

## How do i document my code?
We believe in self documenting code. This means that your code should be written in a way that is easy to understand without the need for extensive comments. However, we do encourage you to add comments where necessary to explain complex logic or decisions.

When documenting your code, please follow these guidelines:
- Use clear and descriptive variable and function names.
- Write concise and meaningful comments that explain the purpose of functions and classes.
- Use docstrings to document the purpose and usage of functions and classes.
- Follow the existing documentation style used in the project.
- Update any relevant documentation files to reflect changes made in the code.
- *DO NOT OVERCOMMENT*

## Coding Standards
We provide a clang-format configuration file in the root directory of the repository. Please use this file to format your code before submitting a pull request. This helps ensure consistency across the codebase and editors.
- Member variables should be prefixed with "m_"
- Static member variables should be prefixed with "s_"
- Global variables should be prefixed with "g_"
- Use PascalCase or UpperCamelCase for variable and function names.
- Pointers should still be pointers when using a typedef (eg. "using ChunkColumnPtr = ChunkColumn*;") (this is optional but preferred)

## UI/UX Standards
When designing user interfaces, please follow these guidelines:
- Maintain a consistent color scheme and typography throughout the project.
- Ensure that all UI elements are easily readable and accessible.
- Follow OreUI/new Bedrock Edition design patterns and conventions for user interfaces.
- Use the given template to ensure proper sizing and alignment of UI elements.
- Everything should always be a multiple of 2 pixels (2px, 4px, 6px, ... ) unless otherwise specified.

## WHAT NOT TO DO
- NO MESSING WITH THE BUILD SYSTEM (adding new cpp files and/or removing them, similar to submodules is allowed) 
- NO ADDING NEW DEPENDENCIES without prior approval from the maintainers.
- NO ADDITIONAL PROGRAMMING LANGUAGES (Scripting languages as well)
- NO AI CODE UNLESS ALLOWED AND REVIEWED BY A MAINTAINER
- NO MINIFICATION/OBFUSCATION OF CODE
- NO *forced* SPAGHETTI CODE (keep functions and classes focused on a single responsibility)
- NO GINORMOUS PR'S (try to keep them small and focused on a single issue/feature)
- NO FORCE PUSHES / PR COMMIT OVERRIDES (We want a history of all the changes)
