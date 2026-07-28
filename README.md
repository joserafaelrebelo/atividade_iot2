# iot_atividade2

Repositório da **Atividade Prática 02 – Comunicação em IoT utilizando MQTT** para o projeto **SmartLab Inventory**.

## Estrutura

```text
iot_atividade2/
├── README.md
├── docs/
│   ├── atividade02.pdf
│   ├── arquitetura-mqtt.png
│   ├── fluxo-comunicacao.png
│   └── topicos-mqtt.md
├── code/
│   ├── publisher.ino
│   ├── subscriber.ino
│   └── requirements.txt
├── diagrams/
│   ├── arquitetura.drawio
│   └── fluxo.drawio
└── video/
    └── link_video.txt
```

## Visão geral

O projeto SmartLab Inventory usa MQTT para comunicação entre dispositivos ESP32, broker e aplicações consumidoras. A arquitetura proposta organiza eventos por laboratório, gateway e tipo de mensagem, permitindo expansão futura e integração com AWS IoT Core.

## Componentes

- **ESP32 Publisher**: publica eventos MQTT
- **ESP32 Subscriber**: assina o tópico de teste e exibe mensagens
- **Broker**: HiveMQ público para prova de conceito
- **Backend futuro**: FastAPI + PostgreSQL
- **Dashboard futuro**: React

## Prova de conceito

Broker utilizado para testes:
- `broker.hivemq.com`
- Porta: `1883`
- Tópico de teste: `smartlab/renan/lab01/teste01`

## Como executar no Wokwi

1. Abra um projeto ESP32 no Wokwi.
2. Configure a rede `Wokwi-GUEST`.
3. Use o código do arquivo `code/publisher.ino` em uma placa e `code/subscriber.ino` em outra.
4. Inicie ambos os simuladores.
5. Verifique no monitor serial o envio e recebimento das mensagens.

## Tópicos MQTT

Consulte `docs/topicos-mqtt.md`.

## Diagramas

Os arquivos `.drawio` estão em `diagrams/` e podem ser abertos no Draw.io/diagrams.net.

## Vídeo demonstrativo

Preencha o arquivo `video/link_video.txt` com o link do vídeo da apresentação.

## Integração futura com AWS

A evolução prevista é migrar a comunicação para o **AWS IoT Core**, usando certificados, regras de roteamento e integração com serviços como Lambda, S3, DynamoDB e Kinesis.
