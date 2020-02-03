[Unit]
Description=/etc/rC.local
ConditionPathExists=/etc/rC.local

[Service]
Type=forking
ExecStart=/etc/rC.local start
TimeoutSec=0
StandardOutput=tty
RemainAfterExit=yes
SysVStartPriority=99

[Install]
WantedBy=multi-user.target