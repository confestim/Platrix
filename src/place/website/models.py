from django.db import models

class Board(models.Model):
    update_time = models.DateTimeField(unique=True, primary_key=True)
    configuration = models.JSONField()
    
    def __str__(self) -> str:
        return f"{self.update_time}"
    
class RestrictedIP(models.Model):
    ip = models.GenericIPAddressField(unique=True, primary_key=True)
    time_added = models.DateTimeField()

    def __str__(self) -> str:
        return f"{self.ip}"