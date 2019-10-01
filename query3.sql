select distinct city.city_id, city.city
	from city , address,  customer, rental, inventory, film_actor,actor
	where  address.address_id = customer.address_id and 
	customer.customer_id = rental.customer_id and 
	rental.inventory_id = inventory.inventory_id and 
	inventory.film_id = film_actor.film_id and 
	film_actor.actor_id = actor.actor_id and 
	actor.first_name = 'Bob' and 
	actor.last_name = 'Fawcett' and 
	city.city_id = address.city_id
	order by city