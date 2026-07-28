# Atividade Prática 02 – Comunicação em IoT utilizando MQTT

Este documento resume os tópicos MQTT definidos para o projeto SmartLab Inventory.

## Tópicos propostos

| Tópico | Publisher | Subscriber | Finalidade |
| :-- | :-- | :-- | :-- |
| `smartlab/lab01/gateway01/status` | ESP32 | Backend, dashboard | Informar estado do dispositivo, conectividade e disponibilidade. |
| `smartlab/lab01/gateway01/rfid` | ESP32 | Backend | Publicar leituras de etiquetas RFID associadas aos itens. |
| `smartlab/lab01/gateway01/door` | ESP32 | Backend, dashboard | Registrar abertura e fechamento de gavetas e armários. |
| `smartlab/lab01/gateway01/weight` | ESP32 | Backend | Informar variações de peso para validar estoque. |
| `smartlab/lab01/inventory/events` | Backend | Dashboard | Publicar eventos consolidados de retirada ou devolução. |
| `smartlab/lab01/inventory/alerts` | Backend | Dashboard, notificações | Alertas de estoque crítico, atraso e inconsistência. |
| `smartlab/lab01/inventory/maintenance` | Backend | Dashboard, manutenção | Eventos associados à manutenção preventiva. |
| `smartlab/lab01/dashboard/commands` | Dashboard | Backend ou gateway | Canal futuro para comandos e sincronização. |

## Estratégia de organização

A estrutura por laboratório, gateway e tipo de evento facilita escalabilidade, manutenção e expansão futura para múltiplos ambientes e dispositivos.
