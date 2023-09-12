# Contributing

Contributions are always welcome and help the project forward. Here are some rules which will help you through the process of submitting your code.

## Git(Hub)

This project is hosted on GitHub and leverages many of GitHub's features. To prevent the GitHub repository from becoming a mess, there are specific guidelines for contributing to this project:

- The stable branch is the destination for all reviewed and thoroughly tested code. A maintainer of this project reviews and incorporates the code from the main branch through a pull request.

- The main branch serves as an integration/beta branch. New code from topic branches undergoes review and is merged into the main branch via a pull request.

- Topic branches are reserved for developing new drivers and typically contain untested, non-production-ready code.

Another important thing to mention are GitHub workflows. This repository has multiple GitHub workflows set-up. Currently it has:

- One workflow for automatically generating doxygen documentation and markdown documentation up on the GitHub pages Wiki.

- One workflow for deploying the GitHub pages wiki itself.

- One workflow for compilation checking/static analysis.

### Submitting your code

When engaging in this project, we highly recommend the following process: begin by forking the project, then proceed to develop your driver within your fork. Finally, submit your code to the main branch of this project using a pull request.

Additionally, if you wish to introduce a new concept or idea before diving into the implementation, you have the option to create a GitHub Issue to discuss it.

In terms of code reviews, please be prepared for your code and ideas to undergo thorough examination, often resulting in constructive criticism and suggestions for improvement. This review process may require you to make adjustments to your code before it can be merged with the main project. It's important to understand that this critique is aimed at achieving the best possible solution for the Universal HAL project. Don't be discouraged by criticism; everyone involved shares the common goal of enhancing the project.

If you have any questions or need clarification regarding your contributions, feel free to create an issue on the Universal HAL GitHub repository page.



## Code style



## Software principles

When contributing I generally recommend following these principles:

- KISS Principle:
  
  "Keep It Simple Stupid"

- Loose Coupling (But don't over-exaggerate):
  
  "Components are weakly associated (have breakable relationships) with each other, and thus changes in one component least affect existence or performance of another component."

- DRY:
  
  "Don't repeat yourself"

- Fail-fast:
  
  "A **fail-fast** system is one which immediately reports at its interface any condition that is likely to indicate a failure."

- Minimalism:
  
  "**Minimalism** refers to the application of [minimalist](https://en.wikipedia.org/wiki/Minimalist "Minimalist") philosophies and principles in the design and use of hardware and software. Minimalism, in this sense, means designing systems that use the least hardware and software resources possible."
