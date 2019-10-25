
select s5.name
from 
	(select category.name, count(category.name) as nums
	from category, film_category, inventory, rental,
		(select s3.customer_id
		from
			(select customer_id, count(inventory_id) as nums
				from rental
				group by customer_id
				order by nums desc
				limit 1) as s2,
			(select customer_id, count(inventory_id) as nums
				from rental
				group by customer_id
				) as s3
		where s2.nums = s3.nums) as s4
	where category.category_id = film_category.category_id and
	film_category.film_id = inventory.film_id and
	inventory.inventory_id = rental.inventory_id and
	rental.customer_id = s4.customer_id
	group by category.name
	order by nums desc) as s5,
	
	(select category.name, count(category.name) as nums
	from category, film_category, inventory, rental,
		(select s3.customer_id
		from
			(select customer_id, count(inventory_id) as nums
				from rental
				group by customer_id
				order by nums desc
				limit 1) as s2,
			(select customer_id, count(inventory_id) as nums
				from rental
				group by customer_id
				) as s3
		where s2.nums = s3.nums) as s4
	where category.category_id = film_category.category_id and
	film_category.film_id = inventory.film_id and
	inventory.inventory_id = rental.inventory_id and
	rental.customer_id = s4.customer_id
	group by category.name
	order by nums desc
	limit 1) as s6

	where s6.nums = s5.nums

