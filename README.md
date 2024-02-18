# Reparo

Reparo is a local-based software which allows repair shops to take control over their repair orders.
Track you repairs, add, delete, edit them with ease.

## Installation

With first release `.exe` will be provided.

## Features

- **Platform:** Windows
- **User-Friendly:** Quickly add new repair ticket
- **Devices database:** Includes a database of the most popular mobile phones, complete with color options
- **Repairs status:** Easily sort through repair orders by status for a clearer overview of your workload
- **Calendar:** Organize repairs based on dates
- **Notes:** Enables adding notes that can either be visible to the customer or hidden
- **Edit repair:** Modify repair details with a history log for reference
- **Delete repair:** Remove unwanted repair orders from the system
- **Finances:**  Monitor your earnings through the finances feature
- **Themes:** Choose between Dark and Light modes for your interface

### Adding a Customer
When adding a new customer, the software will alert you if the customer already exists in the database. This feature is particularly useful as it is uncommon to add a customer without an associated repair order. A red or green indicator bar will notify the user whether the customer details have been properly filled in. 

`Upcoming Feature` These fields will be customizable, allowing users to decide which details are necessary, as some shops may opt not to collect comprehensive data from their customers.

![image](https://github.com/damian-kos/reparo/assets/106775028/889a7fe1-5113-4d38-9a2a-c92a18f5c557)
![image](https://github.com/damian-kos/reparo/assets/106775028/f7e04f59-5817-4e63-b433-c8250a8f9b3e)
![image](https://github.com/damian-kos/reparo/assets/106775028/2b476044-c156-46a3-b3e4-34dd9764a952)

### Adding a Repair
When adding a new repair, you will see a window with Customer details and Device details. 
- **Customer section:** This section automatically searches the existing customer database using the phone number, which serves as a relatively unique identifier. Selecting a customer from the popup window automatically fills in the rest of their details.

![image](https://github.com/damian-kos/reparo/assets/106775028/92ddb7a1-446b-4091-9bd7-d40803f1283d)
![image](https://github.com/damian-kos/reparo/assets/106775028/337dc285-6f10-440c-a103-139b00afa797)
![image](https://github.com/damian-kos/reparo/assets/106775028/ec81bb8b-46de-408d-8f59-b91a2f9eb420)


- **Device section:** This part allows you to search for devices using the comprehensive database, which includes device names sourced directly from official manufacturer data whenever possible.
  -  `Upcoming Feature`  Soon, you will be able to add a custom device, offering more flexibility to accommodate a wide range of repairs.

![image](https://github.com/damian-kos/reparo/assets/106775028/1c1796e0-5cc3-4efe-bba3-f973a9b2fb5f)
- As you type, a list of available devices will be suggested.

![image](https://github.com/damian-kos/reparo/assets/106775028/c810ae75-05e4-449a-b21d-46a5b5bf34af)

- Upon selecting a device, if the database contains color options specific to that model, these will be displayed for selection.
  
![image](https://github.com/damian-kos/reparo/assets/106775028/5b50935b-4b21-4010-8992-395035c083a9)

- **SN / IMEI section:** You have the option to enter the serial number or IMEI of the device.
  - `Upcoming Feature` This field will be made editable, allowing you to require it as necessary for your record-keeping.
- **Notes section:** You can add notes to each repair. One note is visible to the customer and will be printed on the ticket, while the other is private.
  - `Upcoming Feature` his section will be made editable, enabling you to require notes as needed.
- **Submit section:** Once all fields are completed and validated, the "Insert Repair" button will become active. Clicking this button will prompt a pop-up where you can confirm the addition of the repair or cancel the action.
  - `Upcoming Feature` **Print Repair Ticket** A feature to print the Repair Ticket, which can be handed to your customer for their reference.
  - `Upcoming Feature` **Print Barcode** An option to print a Barcode for the repair, facilitating easier tracking and management of repair jobs.

### Edit Repair
The Edit Repair feature allows you to modify the details of an existing repair, including its status, price, or associated customer information. Here's how updates to customer details are handled:
- If you modify the details of the current customer and the new details do not match the phone number of any other customer in our database, the current customer's record will be updated with the new information.
- If the new details entered match the phone number of another customer in our database, the repair will then be associated with this other customer.

![image](https://github.com/damian-kos/reparo/assets/106775028/bc1018c1-786c-497b-846a-1577cc319abf)

All changes made to a repair order are documented in the Updates section. This ensures a clear history of modifications for each repair, providing transparency and accountability for both the repair shop and its customers.

 ![image](https://github.com/damian-kos/reparo/assets/106775028/dd3c680e-4f1a-413d-8f2f-19286bf14451)


### Repairs View
In the Repairs View, you have the ability to browse through all the repairs recorded in your system. Repairs are organized by their current status, but you can also filter them based on specific dates:

![image](https://github.com/damian-kos/reparo/assets/106775028/b59e7ea0-b5fe-44fe-9412-88d4ec9c139b)

- Before a specified *date*
- After a specified *date*
- Between two specified *dates*

![image](https://github.com/damian-kos/reparo/assets/106775028/160bd43f-e2f9-4d4c-994e-d1474d6fa007)

The table displaying the repairs can be sorted by the following criteria:
`ID` `Model` `Category` `Price` `Date` 
You have the flexibility to hide certain columns or rearrange their order according to your preferences.

![image](https://github.com/damian-kos/reparo/assets/106775028/bc187dae-a17a-4c8b-94fc-eaaa9f41f9ec)


Right-clicking on a repair entry will bring up a popup menu that provides options to **Delete** or **Edit** the selected repair. This feature enables quick and easy management of repair orders directly from the Repairs View.

![image](https://github.com/damian-kos/reparo/assets/106775028/8d20581d-9365-4c19-9bfa-f129319971db)


#### Upcoming Features
Stay tuned for these upcoming features to further enhance the functionality and user experience of Reparo.

- `Upcoming Feature` **Insert Customer** fields will be customizable, allowing users to decide which details are necessary, as some shops may opt not to collect comprehensive data from their customers.
- `Upcoming Feature` **Devices** Soon, you will be able to add a custom device, offering more flexibility to accommodate a wide range of repairs.
- `Upcoming Feature` **SN / IMEI** This field will be made editable, allowing you to require it as necessary for your record-keeping.
- `Upcoming Feature` **Notes section** This section will be made editable, enabling you to require notes as needed.
- `Upcoming Feature` **Print Repair Ticket** A feature to print the Repair Ticket, which can be handed to your customer for their reference.
- `Upcoming Feature` **Print Barcode** An option to print a Barcode for the repair, facilitating easier tracking and management of repair jobs.

