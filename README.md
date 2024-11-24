# **Kodiak Spoofer**

## Description

The **Kodiak Spoofer** is a tool that helps protect your privacy by spoofing key identifiers used by Minecraft Bedrock Edition. The tool modifies:
- **SSID** (SelfSignedId)
- **DID** (DeviceId)
- **CID** (ClientRandomId)

Additionally, it removes various game-related files and clears cached data that may be used to track or identify the user. After performing these tasks, the tool automatically launches Minecraft Bedrock Edition.

## How It Works

When launched, the spoofer performs the following actions:
1. **File Removal**: Deletes specific files and cached data that can be used for tracking or identification.
2. **Spoofs Identifiers**: The tool replaces the **SSID**, **DID**, and **CID** with new, randomly generated values to make the session appear as if it's coming from a different device or client.
3. **Launches Minecraft**: Once the spoofing process is complete, the tool automatically opens Minecraft Bedrock Edition.

After Minecraft is launched, you will be prompted to confirm by clicking "OK" when you're in the main menu and connected to Xbox Live.

## Usage

### 1. **Build the Project**

Ensure that the project is compiled. If you don’t have the executable (`Spoofer.exe`), you will need to compile it using Visual Studio. Once compiled, proceed to the next step.

### 2. **Running the Spoofer**

To use the spoofer, run the executable:

```bash
Spoofer.exe
```

This will:
- Remove specific files and cached data.
- Spoof the **SSID**, **DID**, and **CID** to new values.
- Automatically launch Minecraft Bedrock Edition.

### 3. **After Launching Minecraft**

Once Minecraft is running:
- Wait until you’re connected to Xbox Live.
- Click "OK" in the message box that appears to confirm the spoofing was successful.

### 4. **What the Spoofer Does**

The spoofer:
- **Removes** certain files and cached data to ensure privacy.
- **Spoofs** key identifiers (SSID, DID, CID) to mask your actual session, device, and client.

## Important Notes:
- **Use responsibly**: This tool is intended for educational purposes. Ensure you comply with the terms of service of Minecraft or any platform you’re interacting with.

## License

This project is licensed under the **Apache License 2.0**. See the [LICENSE](LICENSE) file for more details.