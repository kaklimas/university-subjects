EX1 

z1
SELECT OrderID, SUM(UnitPrice * Quantity * (1-Discount)) as 'value' FROM [Order Details]
GROUP BY OrderID
ORDER BY value DESC

z2
SELECT TOP 10 OrderID, SUM(UnitPrice * Quantity * (1-Discount)) as 'value' FROM [Order Details]
GROUP BY OrderID
ORDER BY value DESC

EX2

z1
SELECT ProductID, COUNT(*) as 'num' FROM [Order Details]
WHERE ProductID < 3
GROUP BY ProductID

z2
SELECT ProductID, SUM(Quantity) as 'total_quantity' FROM [Order Details]
GROUP BY ProductID
ORDER BY ProductID

z3
SELECT OrderID, SUM(Quantity) as 'total_quantity', SUM(UnitPrice * Quantity * (1-Discount)) as 'total_order_val' FROM [Order Details]
GROUP BY OrderID
HAVING SUM(Quantity) > 250

EX2

z1
SELECT EmployeeID, COUNT(*) as 'num_of_orders' FROM Orders
GROUP BY EmployeeID

z2
SELECT ShipVia, OrderID, SUM(Freight) as 'opłata_za_przesyłkę' FROM Orders
GROUP BY ShipVia, OrderID
WITH ROLLUP
ORDER BY ShipVia, OrderID

z3
SELECT ShipVia, OrderID, SUM(Freight) as 'total_freight' FROM Orders
WHERE YEAR(ShippedDate) IN (1996, 1997)
GROUP BY ShipVia, OrderID
WITH ROLLUP
ORDER BY ShipVia, OrderID

EX4

z1
SELECT EmployeeID, YEAR(ShippedDate) as 'year', MONTH(ShippedDate) as 'month', COUNT(*) as 'total_orders' FROM Orders
GROUP BY EmployeeID, YEAR(ShippedDate), MONTH(ShippedDate)
WITH ROLLUP
ORDER BY EmployeeID, year, month

z2
SELECT CategoryID, MAX(UnitPrice) as 'max_val', MIN(UnitPrice) as 'min_val' FROM Products
GROUP BY CategoryID
ORDER BY CategoryID
