select customer.customer_id, customer.first_name, customer.last_name 
from customer,
	(select s1.customer_id
	from
		(select customer_id, count(inventory_id) as nums
		from rental
		group by customer_id) as s1,
		
		(select customer_id, count(inventory_id) as nums
		from rental
		group by customer_id
		order by nums desc
		limit 1) as s2

	where s1.nums = s2.nums) as s3

where customer.customer_id = s3.customer_id