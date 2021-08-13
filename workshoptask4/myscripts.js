// Initialize Firestore through Firebasevar 
firebaseConfig = {
	apiKey: "AIzaSyDCBEHLvYWJ0c6Oc5WeqgdjP9VMDjmWK0s",
	authDomain: "pacific-castle-310617.firebaseapp.com",
	projectId: "pacific-castle-310617",
	storageBucket: "pacific-castle-310617.appspot.com",
	messagingSenderId: "172508040408",
	appId: "1:172508040408:web:9191a9eb64ad1c7976dbad"
	};
	
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

var db = firebase.firestore();	

// Live data snapshot, i.e. auto-refresh
db.collection("Reviews").onSnapshot((querySnapshot) => {

	// Empty HTML table
	$('#reviewList').empty();
	
	// Loop through snapshot data and add to HTML table
	querySnapshot.forEach((doc) => {
		$('#reviewList').append('<tr>');
			$('#reviewList').append('<td>'  + doc.data().book_name + '</td>');
			$('#reviewList').append('<td>'  + doc.data().book_rating + '/5</td>');
			$('#reviewList').append('<td>'  + doc.data().review_date + '</td>');
			$('#reviewList').append('<td> <button type="button" onclick="deleteRow(this)" value="'+doc.id+'" class="btn btn-primary" id="removeButton">Delete</button> </td>')
		$('#reviewList').append('</tr>');
		
	});
	
	// Display review count
	$('#mainTitle').html(querySnapshot.size + " book reviews in the list");
});

// Add button pressed
$("#addButton").click(function() {
	//get the current date and time
	var today = new Date();
	var date = today.getDate()+'-'+(today.getMonth()+1)+'-'+today.getFullYear();
	var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
	var dateTime = date+' '+time;

	// Add review to Firestore collection
	db.collection("Reviews").add({
		book_name: $("#bookName").val(),
		book_rating: parseInt($("#bookRating").val()),
		review_date: dateTime
		})
		.then((docRef) => {
			console.log("Document written with ID: ", docRef.id);
		})
		.catch((error) => {
			console.error("Error adding document: ", error);
			});
			// Reset form
			$("#bookName").val('');
			$("#bookRating").val('1');
	});

// Delete button pressed
function deleteRow(id){
	db.collection("Reviews").doc(""+id.value).delete().then(() => {
		console.log("Document successfully deleted!");
	}).catch((error) => {
		console.error("Error removing document: ", error);
	});

}

	
