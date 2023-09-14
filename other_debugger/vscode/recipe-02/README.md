调试器运行程序的环境变量, 通过launch.json中的envFile属性指定
{
    "configurations": [
        {
            "envFile": "${workspaceFolder}/.env",
            // ...
        }
    ]
}

envFile的内容格式为每行KEY=VALUE
