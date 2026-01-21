# Vinilcius

> **Final Project – Programming 3**  
> **Professor:** Ruben Carlo Benante  
> **Institution:** Polytechnic School of Pernambuco (POLI/UPE)

## About the Project

**Vinilcius** is a desktop application developed in **C++** using the **Qt Framework**, designed to fully manage a vinyl record store.

The system simulates a sales environment, allowing the commercialization of vinyl records
(new and used) and providing distinct interfaces for **Customers** and
**Employees/Administrators**, ensuring data security and organization through a
relational database.

---

## Test Users

### Customer
- **User:** caio@gmail.com  
- **Password / CPF:** 16964010427  

### Employee
- **User:** fab@vinilcius.com  
- **Password / CPF:** 6655987413  

### Manager
- **User:** Rodri@vinilcius.com  
- **Password / CPF:** 71393581412  

### Software Admin – First Registration
- **User:** Registration allowed with any email  
- **Password:** (*****)
- The password will be defined by the Software Admin during the first login.

---

## Technologies Used

- **Language:** C++
- **Graphical Interface:** Qt Framework (Qt Designer)
- **Database:** SQLite
- **Recommended IDE:** Qt Creator

---

## ⚙️ Architecture and Data Modeling

The project was structured with a focus on separation of responsibilities and data persistence.

### Database (SQLite)

The system uses a relational database composed of the following main tables:

- `TB_USERS`: Base table for login and authentication  
- `TB_CUSTOMERS`: Personal data of buyers  
- `TB_EMPLOYEES`: Administrative staff data  
- `TB_RECORDS`: Product catalog (includes New/Used flag, Genre, Artist)  
- `TB_STOCK`: Inventory control of records  
- `TB_SALES`: Record of completed transactions  

---

## Features

The system includes an access control mechanism via login, redirecting users to the
appropriate interface based on their permission level.

### Customer Module
- **Catalog Browsing:** View available vinyl records  
- **Filters:** Search for new or used records  
- **Shopping Cart:** Add and remove items before checkout  

### Administrative Module (Employee)
- **Record Management:** Register new titles, edit prices, and remove items  
- **Stock Management:** Increase or decrease inventory quantities  
- **People Management:** Register and remove employees and view customer lists  
- **Dashboard:** View useful information (sales reports, best-selling items, etc.)

---

## Authors

- Gabriel Dutra Chaves           | <gdc1@poli.br>  | @dutraat  
- Vinicius Magalhães de Medeiros | <vmm1@poli.br>  | @Viniciusmedeiros1  
- Juliana Silva Fonseca Carneiro | <jsfc1@poli.br> | @juliana373217  
- Anthony Lucas dos Santos Silva | <alss1@poli.br> | @Anthonyldss  
- Diego Nascimento dos Santos    | <dns@poli.br>   | @Dinego05  
- Dennis Miller Fidelis da Silva | <dmfs@poli.br>  | @dmfdsilva  
- Igor Guimarães Monteiro        | <igm1@poli.br>  | @igm22  
- Anne Luiza Assunção Freire     | <alaf@poli.br>  | @AnneLuiza  
- Christian Viana Tomaz          | <cvt@poli.br>   | @christianvtomaz  

---
