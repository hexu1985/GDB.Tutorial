VS Code与GDB Server远程调试, 通过launch.json中的miDebuggerServerAddress字段指定
{
    "configurations": [
        {
            // ...
            "miDebuggerServerAddress": "192.168.10.163:12345"
        }
    ]
}
