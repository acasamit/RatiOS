# RatiOS (simple i386 kernel from scratch)

![RatiOS](https://i.imgur.com/mO8gPhw.png)

RatiOS is a simple operating system kernel developed in **C** and **Assembly** for **BIOS** (not EFI) on the **i386 architecture**. It includes core components like a Global Descriptor Table (GDT) and an Interrupt Descriptor Table (IDT), providing a basic foundation for memory segmentation and interrupt handling. 

- **GDT (Global Descriptor Table)**: The GDT manages memory segmentation in protected mode, defining memory segments for code and data.
- **IDT (Interrupt Descriptor Table)**: The IDT handles interrupt vectors, allowing RatiOS to respond to hardware and software interrupts.

## Features

### Basic Shell

RatiOS includes a basic shell with the following commands:

- **`test`**: Runs a test function to print the memory addresses of two hardcoded stack variables.
- **`lock`**: Locks the session; password to unlock is `ratio`.
- **`echo`**: Prints text to the screen.
- **`clear`**: Clears the screen.
- **`exit`**: Turns off the machine.
- **`shutdown`**: Turns off the machine.
- **`halt`**: Halts all system processes.
- **`reboot`**: Restarts the machine.

### Keyboard Shortcuts

- **Ctrl + S (Switch)**: Switches between two available workspaces.
- **Ctrl + C (Color)**: Changes the text color.
- **Ctrl + L (Lock)**: Locks the session.
