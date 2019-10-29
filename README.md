# IPP-projekt1

The goal of the task is to write a program describing the behavior of a particle in an imaginary quantum world. The particle can reside in four quantum states, denoted by the numbers 0, 1, 2 and 3. The quantum history of a particle is a non-empty sequence of consecutive quantum states in which the particle resided, but only some quantum stories of the particle are acceptable. The admissible quantum history of a particle can be attributed to quantized energy. This energy can be changed. An unacceptable quantum history of a particle cannot have energy assigned. The energies of two allowable quantum stories of a particle can be equated, which means that a change in energy for one of these stories also causes a change in the energy of the other. The energy equation relationship attributed to stories is equivalence.

Possible commands:

- DECLARE history - It causes that any quantum history of a particle, which is a prefix of the quantum history of a particle given as the "history" parameter, becomes acceptable.

- REMOVE history - The result is that any quantum history of a particle that has a prefix given as a "history" parameter is no longer acceptable.

- VALID history - Checks if the quantum particle history given is acceptable.

- ENERGY history energy - If the quantum history of a particle given in the "history" parameter is acceptable and the history has no energy assigned, it assigns the energy value given as the "energy" parameter. If this quantum history of a particle already has energy assigned, it modifies the value of that energy.

- ENERGY history - If the quantum history of a particle given in the "history" parameter is acceptable and the history has energy assigned, it prints the value of that energy.

- EQUAL history_a history_b - If the quantum histories given in the parameters "history_a" and "history_b" are acceptable, then: (a) if at least one of them has energy assigned, it balances their energies and closes the equivalence relationship; (b) if both stories have energy assigned to them prior to carrying out this command, then after the command the energy assigned to these stories is the arithmetic mean of these energies (with rounding down if necessary); (c) if both stories are identical or already have equal energies, it doesn't change anything.

Valid input data meets the following additional conditions:

- The parameter describing the quantum history of a particle is a non-empty string consisting of the numbers 0, 1, 2 and 3.
- Energy is a decimal integer between 1 and 2 ^ 64 - 1.
- The command name and its parameters are separated by a single space, and each input line contains at most one command and ends with a linux character transition to the new line (character \ n in C, ASCII code 10). These are the only whitespace in the line.

Also:
Blank lines should be ignored.
Lines beginning with the # character should be ignored.

The program acknowledges the correct execution of the command, writing information to the standard output:

For a command other than the VALID command or the single parameter ENERGY command, the line with the word OK.
For the VALID command, the line with the word YES or NO depending on the result of the command.
For one parameter ENERGY command, a decimal integer that is the energy value.
Each output line should end with a Linux newline character (\ n in C, ASCII code 10). This is the only white sign that can appear on the exit.
The program prints error information to the standard diagnostic output:

For each incorrect line and for each command that cannot be executed because it does not meet the conditions described above, e.g. due to the wrong number of parameters or an incorrect parameter value, a line with the word ERROR should be printed, followed by a Linux end of line character (sign \n in C, ASCII code 10). This is the only white sign that can appear on the exit.
